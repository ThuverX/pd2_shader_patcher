#include "superblt_flat.h"

#include "xml.h"
#include "file.h"
#include <filesystem>

static void handle_mxml_error_crash(const char* error)
{
	std::string msg = "[XML] mxml error: " + std::string(error);
	PD2HOOK_LOG_ERROR(msg.c_str());
}

ShaderPassSamplerPatch* getPatchSamplerFromXML(mxml_node_t* samplerNode)
{
	ShaderPassSamplerPatch* patch = new ShaderPassSamplerPatch();

	const char* samplerId = mxmlElementGetAttr(samplerNode, "id");

	if (!samplerId)
	{
		PD2HOOK_LOG_ERROR("[XML] Sampler missing id!");
		return nullptr;
	}

	patch->id = std::stoi(samplerId);

	mxml_node_t* varNode;

	for (varNode = mxmlFindElement(samplerNode, samplerNode, "var", NULL, NULL, MXML_DESCEND); varNode != NULL;
	     varNode = mxmlFindElement(varNode, samplerNode, "var", NULL, NULL, MXML_DESCEND))
	{
		const char* id = mxmlElementGetAttr(varNode, "id");
		const char* value = mxmlElementGetAttr(varNode, "value");

		if (!id || !value)
		{
			PD2HOOK_LOG_ERROR("[XML] Sampler var missing id or value!");
			continue;
		}

		patch->vars[std::stoi(id)] = std::stoi(value);
	}

	return patch;
}

std::string getFullPath(std::string xmlPath, std::string path) {
    std::filesystem::path xmlPathDir = std::filesystem::path(xmlPath).parent_path();

    std::filesystem::path fullPath = xmlPathDir / path;

    return fullPath.string();
}

ShaderPassPatch* getPatchPassFromXML(mxml_node_t* passNode, std::string xmlPath)
{
	ShaderPassPatch* patch = new ShaderPassPatch();
	mxml_node_t* samplersNode = mxmlFindElement(passNode, passNode, "samplers", NULL, NULL, MXML_DESCEND);

	if (samplersNode)
	{
		mxml_node_t* samplerStateNode;

		for (samplerStateNode = mxmlFindElement(samplersNode, samplersNode, "patch", NULL, NULL, MXML_DESCEND);
		     samplerStateNode != NULL;
		     samplerStateNode = mxmlFindElement(samplerStateNode, samplersNode, "patch", NULL, NULL, MXML_DESCEND))
		{
			patch->samplers.push_back(getPatchSamplerFromXML(samplerStateNode));
		}
	}

	mxml_node_t* renderStatesNode = mxmlFindElement(passNode, passNode, "render_states", NULL, NULL, MXML_DESCEND);

	if (renderStatesNode)
	{
		mxml_node_t* varNode;

		for (varNode = mxmlFindElement(renderStatesNode, renderStatesNode, "var", NULL, NULL, MXML_DESCEND);
		     varNode != NULL; varNode = mxmlFindElement(varNode, renderStatesNode, "var", NULL, NULL, MXML_DESCEND))
		{
			const char* id = mxmlElementGetAttr(varNode, "id");
			const char* value = mxmlElementGetAttr(varNode, "value");

			if (!id || !value)
			{
				PD2HOOK_LOG_ERROR("[XML] Render state var missing id or value!");
				continue;
			}

			patch->render_states[std::stoi(id)] = std::stoi(value);
		}
	}

	mxml_node_t* pixelShaderNode = mxmlFindElement(passNode, passNode, "pixel_shader", NULL, NULL, MXML_DESCEND);

	if (!pixelShaderNode)
	{
		PD2HOOK_LOG_ERROR("[XML] Pixel shader missing!");
		return nullptr;
	}

	const char* pixelShaderPath = mxmlElementGetAttr(pixelShaderNode, "path");

	std::vector<unsigned char> pixelShaderContent = file_read(getFullPath(xmlPath, std::string(pixelShaderPath)));

	patch->pixel_shader = pixelShaderContent;

	mxml_node_t* vertexShaderNode = mxmlFindElement(passNode, passNode, "vertex_shader", NULL, NULL, MXML_DESCEND);

	if (!vertexShaderNode)
	{
		PD2HOOK_LOG_ERROR("[XML] Vertex shader missing!");
		return nullptr;
	}

	const char* vertexShaderPath = mxmlElementGetAttr(vertexShaderNode, "path");

	std::vector<unsigned char> vertexShaderContent = file_read(getFullPath(xmlPath, std::string(vertexShaderPath)));

	patch->vertex_shader = vertexShaderContent;

	return patch;
}

std::vector<ShaderPatch*> getPatchesFromXML(std::string file)
{
	std::vector<ShaderPatch*> patches = std::vector<ShaderPatch*>();
	std::vector<unsigned char> content = file_read(file);

    PD2HOOK_LOG_LOG(file.c_str());

	mxmlSetErrorCallback(handle_mxml_error_crash);

	mxml_node_t* tree = mxmlLoadString(NULL, (char*)content.data(), MXML_NO_CALLBACK);
	mxml_node_t* root = mxmlFindElement(tree, tree, "patches", NULL, NULL, MXML_DESCEND);

	if (!root)
	{
		PD2HOOK_LOG_ERROR("[XML] No root element found!");
		return std::vector<ShaderPatch*>();
	}

	mxml_node_t* patchNode;

	for (patchNode = mxmlFindElement(root, root, "patch", NULL, NULL, MXML_DESCEND); patchNode != NULL;
	     patchNode = mxmlFindElement(patchNode, root, "patch", NULL, NULL, MXML_DESCEND))
	{
		const char* apply_to = mxmlElementGetAttr(patchNode, "apply_to");
		const char* name = mxmlElementGetAttr(patchNode, "name");
		const char* mode = mxmlElementGetAttr(patchNode, "mode");

		if (!apply_to || !name || !mode)
		{
			PD2HOOK_LOG_ERROR("[XML] Patch is missing attributes!");
			continue;
		}

		ShaderPatch* patch = new ShaderPatch();
		patch->apply_to = std::string(apply_to);
		patch->name = std::string(name);

		if (strcmp(mode, "replace") == 0)
		{
			patch->mode = SHADER_PATCH_MODE_REPLACE;
		}
		else if (strcmp(mode, "append") == 0)
		{
			patch->mode = SHADER_PATCH_MODE_APPEND;
		}
		else
		{
			PD2HOOK_LOG_ERROR("[XML] Patch has invalid mode!");
			continue;
		}

		mxml_node_t* passesNode;

		for (passesNode = mxmlFindElement(patchNode, patchNode, "passes", NULL, NULL, MXML_DESCEND); passesNode != NULL;
		     passesNode = mxmlFindElement(passesNode, patchNode, "passes", NULL, NULL, MXML_DESCEND))
		{
			std::vector<ShaderPassPatch*> passes = std::vector<ShaderPassPatch*>();

			const char* passName = mxmlElementGetAttr(passesNode, "name");

			if (!passName)
			{
				PD2HOOK_LOG_ERROR("[XML] Pass is missing name!");
				continue;
			}

			std::string passNameStr = std::string(passName);

			mxml_node_t* passNode;

			for (passNode = mxmlFindElement(passesNode, passesNode, "pass", NULL, NULL, MXML_DESCEND); passNode != NULL;
			     passNode = mxmlFindElement(passNode, passesNode, "pass", NULL, NULL, MXML_DESCEND))
			{
				ShaderPassPatch* pass = getPatchPassFromXML(passNode, file);

				if (pass)
				{
					passes.push_back(pass);
				}
			}

			patch->passes[passNameStr] = passes;
		}

		patches.push_back(patch);
	}

	return patches;
}
