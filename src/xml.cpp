#include "superblt_flat.h"

#include "xml.h"
#include "file.h"
#include <filesystem>


int sampler_state_from_string(std::string name)
{
	if (name == "D3DSAMP_ADDRESSU")
		return 1;
	if (name == "D3DSAMP_ADDRESSV")
		return 2;
	if (name == "D3DSAMP_ADDRESSW")
		return 3;
	if (name == "D3DSAMP_BORDERCOLOR")
		return 4;
	if (name == "D3DSAMP_MAGFILTER")
		return 5;
	if (name == "D3DSAMP_MINFILTER")
		return 6;
	if (name == "D3DSAMP_MIPFILTER")
		return 7;
	if (name == "D3DSAMP_MIPMAPLODBIAS")
		return 8;
	if (name == "D3DSAMP_MAXMIPLEVEL")
		return 9;
	if (name == "D3DSAMP_MAXANISOTROPY")
		return 10;
	if (name == "D3DSAMP_SRGBTEXTURE")
		return 11;
	if (name == "D3DSAMP_ELEMENTINDEX")
		return 12;
	if (name == "D3DSAMP_DMAPOFFSET")
		return 13;

    return -1;
}

int render_state_from_string(std::string name)
{
	if (name == "D3DRS_ZENABLE")
		return 7;
	if (name == "D3DRS_FILLMODE")
		return 8;
	if (name == "D3DRS_SHADEMODE")
		return 9;
	if (name == "D3DRS_ZWRITEENABLE")
		return 14;
	if (name == "D3DRS_ALPHATESTENABLE")
		return 15;
	if (name == "D3DRS_LASTPIXEL")
		return 16;
	if (name == "D3DRS_SRCBLEND")
		return 19;
	if (name == "D3DRS_DESTBLEND")
		return 20;
	if (name == "D3DRS_CULLMODE")
		return 22;
	if (name == "D3DRS_ZFUNC")
		return 23;
	if (name == "D3DRS_ALPHAREF")
		return 24;
	if (name == "D3DRS_ALPHAFUNC")
		return 25;
	if (name == "D3DRS_DITHERENABLE")
		return 26;
	if (name == "D3DRS_ALPHABLENDENABLE")
		return 27;
	if (name == "D3DRS_FOGENABLE")
		return 28;
	if (name == "D3DRS_SPECULARENABLE")
		return 29;
	if (name == "D3DRS_FOGCOLOR")
		return 34;
	if (name == "D3DRS_FOGTABLEMODE")
		return 35;
	if (name == "D3DRS_FOGSTART")
		return 36;
	if (name == "D3DRS_FOGEND")
		return 37;
	if (name == "D3DRS_FOGDENSITY")
		return 38;
	if (name == "D3DRS_RANGEFOGENABLE")
		return 48;
	if (name == "D3DRS_STENCILENABLE")
		return 52;
	if (name == "D3DRS_STENCILFAIL")
		return 53;
	if (name == "D3DRS_STENCILZFAIL")
		return 54;
	if (name == "D3DRS_STENCILPASS")
		return 55;
	if (name == "D3DRS_STENCILFUNC")
		return 56;
	if (name == "D3DRS_STENCILREF")
		return 57;
	if (name == "D3DRS_STENCILMASK")
		return 58;
	if (name == "D3DRS_STENCILWRITEMASK")
		return 59;
	if (name == "D3DRS_TEXTUREFACTOR")
		return 60;
	if (name == "D3DRS_WRAP0")
		return 128;
	if (name == "D3DRS_WRAP1")
		return 129;
	if (name == "D3DRS_WRAP2")
		return 130;
	if (name == "D3DRS_WRAP3")
		return 131;
	if (name == "D3DRS_WRAP4")
		return 132;
	if (name == "D3DRS_WRAP5")
		return 133;
	if (name == "D3DRS_WRAP6")
		return 134;
	if (name == "D3DRS_WRAP7")
		return 135;
	if (name == "D3DRS_CLIPPING")
		return 136;
	if (name == "D3DRS_LIGHTING")
		return 137;
	if (name == "D3DRS_AMBIENT")
		return 139;
	if (name == "D3DRS_FOGVERTEXMODE")
		return 140;
	if (name == "D3DRS_COLORVERTEX")
		return 141;
	if (name == "D3DRS_LOCALVIEWER")
		return 142;
	if (name == "D3DRS_NORMALIZENORMALS")
		return 143;
	if (name == "D3DRS_DIFFUSEMATERIALSOURCE")
		return 145;
	if (name == "D3DRS_SPECULARMATERIALSOURCE")
		return 146;
	if (name == "D3DRS_AMBIENTMATERIALSOURCE")
		return 147;
	if (name == "D3DRS_EMISSIVEMATERIALSOURCE")
		return 148;
	if (name == "D3DRS_VERTEXBLEND")
		return 151;
	if (name == "D3DRS_CLIPPLANEENABLE")
		return 152;
	if (name == "D3DRS_POINTSIZE")
		return 154;
	if (name == "D3DRS_POINTSIZE_MIN")
		return 155;
	if (name == "D3DRS_POINTSPRITEENABLE")
		return 156;
	if (name == "D3DRS_POINTSCALEENABLE")
		return 157;
	if (name == "D3DRS_POINTSCALE_A")
		return 158;
	if (name == "D3DRS_POINTSCALE_B")
		return 159;
	if (name == "D3DRS_POINTSCALE_C")
		return 160;
	if (name == "D3DRS_MULTISAMPLEANTIALIAS")
		return 161;
	if (name == "D3DRS_MULTISAMPLEMASK")
		return 162;
	if (name == "D3DRS_PATCHEDGESTYLE")
		return 163;
	if (name == "D3DRS_DEBUGMONITORTOKEN")
		return 165;
	if (name == "D3DRS_POINTSIZE_MAX")
		return 166;
	if (name == "D3DRS_INDEXEDVERTEXBLENDENABLE")
		return 167;
	if (name == "D3DRS_COLORWRITEENABLE")
		return 168;
	if (name == "D3DRS_TWEENFACTOR")
		return 170;
	if (name == "D3DRS_BLENDOP")
		return 171;
	if (name == "D3DRS_POSITIONDEGREE")
		return 172;
	if (name == "D3DRS_NORMALDEGREE")
		return 173;
	if (name == "D3DRS_SCISSORTESTENABLE")
		return 174;
	if (name == "D3DRS_SLOPESCALEDEPTHBIAS")
		return 175;
	if (name == "D3DRS_ANTIALIASEDLINEENABLE")
		return 176;
	if (name == "D3DRS_MINTESSELLATIONLEVEL")
		return 178;
	if (name == "D3DRS_MAXTESSELLATIONLEVEL")
		return 179;
	if (name == "D3DRS_ADAPTIVETESS_X")
		return 180;
	if (name == "D3DRS_ADAPTIVETESS_Y")
		return 181;
	if (name == "D3DRS_ADAPTIVETESS_Z")
		return 182;
	if (name == "D3DRS_ADAPTIVETESS_W")
		return 183;
	if (name == "D3DRS_ENABLEADAPTIVETESSELLATION")
		return 184;
	if (name == "D3DRS_TWOSIDEDSTENCILMODE")
		return 185;
	if (name == "D3DRS_CCW_STENCILFAIL")
		return 186;
	if (name == "D3DRS_CCW_STENCILZFAIL")
		return 187;
	if (name == "D3DRS_CCW_STENCILPASS")
		return 188;
	if (name == "D3DRS_CCW_STENCILFUNC")
		return 189;
	if (name == "D3DRS_COLORWRITEENABLE1")
		return 190;
	if (name == "D3DRS_COLORWRITEENABLE2")
		return 191;
	if (name == "D3DRS_COLORWRITEENABLE3")
		return 192;
	if (name == "D3DRS_BLENDFACTOR")
		return 193;
	if (name == "D3DRS_SRGBWRITEENABLE")
		return 194;
	if (name == "D3DRS_DEPTHBIAS")
		return 195;
	if (name == "D3DRS_WRAP8")
		return 198;
	if (name == "D3DRS_WRAP9")
		return 199;
	if (name == "D3DRS_WRAP10")
		return 200;
	if (name == "D3DRS_WRAP11")
		return 201;
	if (name == "D3DRS_WRAP12")
		return 202;
	if (name == "D3DRS_WRAP13")
		return 203;
	if (name == "D3DRS_WRAP14")
		return 204;
	if (name == "D3DRS_WRAP15")
		return 205;
	if (name == "D3DRS_SEPARATEALPHABLENDENABLE")
		return 206;
	if (name == "D3DRS_SRCBLENDALPHA")
		return 207;
	if (name == "D3DRS_DESTBLENDALPHA")
		return 208;
	if (name == "D3DRS_BLENDOPALPHA")
		return 209;

    return -1;
}

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

        int id_int = sampler_state_from_string(std::string(id));

        try {
            id_int = std::stoi(id);
        } catch (std::invalid_argument) {
            id_int = -1;
        }

		if (id_int == -1 || !value)
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

            int id_int = render_state_from_string(std::string(id));

            try {
                id_int = std::stoi(id);
            } catch (std::invalid_argument) {
                id_int = -1;
            }

			if (id_int == -1 || !value)
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
    const char* pixelShaderEntryPoint = mxmlElementGetAttr(pixelShaderNode, "entry");
    const char* pixelShaderPrecompiled = mxmlElementGetAttr(pixelShaderNode, "precompiled");

    std::string fullPixelShaderPath = getFullPath(xmlPath, std::string(pixelShaderPath));

    ShaderPassShaderPatch* pixelShader = new ShaderPassShaderPatch();

    pixelShader->path = fullPixelShaderPath;
    pixelShader->entry_point = std::string(pixelShaderEntryPoint);
    pixelShader->type = pixelShaderPrecompiled == nullptr ? SHADER_PATCH_TYPE_CODE : SHADER_PATCH_TYPE_PRECOMPILED;

    patch->pixel_shader = pixelShader;

	mxml_node_t* vertexShaderNode = mxmlFindElement(passNode, passNode, "vertex_shader", NULL, NULL, MXML_DESCEND);

	if (!vertexShaderNode)
	{
		PD2HOOK_LOG_ERROR("[XML] Vertex shader missing!");
		return nullptr;
	}

	const char* vertexShaderPath = mxmlElementGetAttr(vertexShaderNode, "path");
    const char* vertexShaderEntryPoint = mxmlElementGetAttr(vertexShaderNode, "entry");
    const char* vertexShaderPrecompiled = mxmlElementGetAttr(vertexShaderNode, "precompiled");
    std::string fullVertexShaderPath = getFullPath(xmlPath, std::string(vertexShaderPath));

    ShaderPassShaderPatch* vertexShader = new ShaderPassShaderPatch();

    vertexShader->path = fullVertexShaderPath;
    vertexShader->entry_point = std::string(vertexShaderEntryPoint);
    vertexShader->type = vertexShaderPrecompiled == nullptr ? SHADER_PATCH_TYPE_CODE : SHADER_PATCH_TYPE_PRECOMPILED;

    patch->vertex_shader = vertexShader;

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

    mxmlDelete(tree);

	return patches;
}