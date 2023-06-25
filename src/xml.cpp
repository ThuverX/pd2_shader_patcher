#include "superblt_flat.h"

#include "xml.h"
#include "file.h"
#include <filesystem>

std::map<const char*, int> D3DSAMPLERSTATETYPE = {
	{"D3DSAMP_ADDRESSU", 1},       {"D3DSAMP_ADDRESSV", 2},      {"D3DSAMP_ADDRESSW", 3},
	{"D3DSAMP_BORDERCOLOR", 4},    {"D3DSAMP_MAGFILTER", 5},     {"D3DSAMP_MINFILTER", 6},
	{"D3DSAMP_MIPFILTER", 7},      {"D3DSAMP_MIPMAPLODBIAS", 8}, {"D3DSAMP_MAXMIPLEVEL", 9},
	{"D3DSAMP_MAXANISOTROPY", 10}, {"D3DSAMP_SRGBTEXTURE", 11},  {"D3DSAMP_ELEMENTINDEX", 12},
	{"D3DSAMP_DMAPOFFSET", 13}};

std::map<const char*, int> D3DRENDERSTATETYPE = {{"D3DRS_ZENABLE", 7},
                                                 {"D3DRS_FILLMODE", 8},
                                                 {"D3DRS_SHADEMODE", 9},
                                                 {"D3DRS_ZWRITEENABLE", 14},
                                                 {"D3DRS_ALPHATESTENABLE", 15},
                                                 {"D3DRS_LASTPIXEL", 16},
                                                 {"D3DRS_SRCBLEND", 19},
                                                 {"D3DRS_DESTBLEND", 20},
                                                 {"D3DRS_CULLMODE", 22},
                                                 {"D3DRS_ZFUNC", 23},
                                                 {"D3DRS_ALPHAREF", 24},
                                                 {"D3DRS_ALPHAFUNC", 25},
                                                 {"D3DRS_DITHERENABLE", 26},
                                                 {"D3DRS_ALPHABLENDENABLE", 27},
                                                 {"D3DRS_FOGENABLE", 28},
                                                 {"D3DRS_SPECULARENABLE", 29},
                                                 {"D3DRS_FOGCOLOR", 34},
                                                 {"D3DRS_FOGTABLEMODE", 35},
                                                 {"D3DRS_FOGSTART", 36},
                                                 {"D3DRS_FOGEND", 37},
                                                 {"D3DRS_FOGDENSITY", 38},
                                                 {"D3DRS_RANGEFOGENABLE", 48},
                                                 {"D3DRS_STENCILENABLE", 52},
                                                 {"D3DRS_STENCILFAIL", 53},
                                                 {"D3DRS_STENCILZFAIL", 54},
                                                 {"D3DRS_STENCILPASS", 55},
                                                 {"D3DRS_STENCILFUNC", 56},
                                                 {"D3DRS_STENCILREF", 57},
                                                 {"D3DRS_STENCILMASK", 58},
                                                 {"D3DRS_STENCILWRITEMASK", 59},
                                                 {"D3DRS_TEXTUREFACTOR", 60},
                                                 {"D3DRS_WRAP0", 128},
                                                 {"D3DRS_WRAP1", 129},
                                                 {"D3DRS_WRAP2", 130},
                                                 {"D3DRS_WRAP3", 131},
                                                 {"D3DRS_WRAP4", 132},
                                                 {"D3DRS_WRAP5", 133},
                                                 {"D3DRS_WRAP6", 134},
                                                 {"D3DRS_WRAP7", 135},
                                                 {"D3DRS_CLIPPING", 136},
                                                 {"D3DRS_LIGHTING", 137},
                                                 {"D3DRS_AMBIENT", 139},
                                                 {"D3DRS_FOGVERTEXMODE", 140},
                                                 {"D3DRS_COLORVERTEX", 141},
                                                 {"D3DRS_LOCALVIEWER", 142},
                                                 {"D3DRS_NORMALIZENORMALS", 143},
                                                 {"D3DRS_DIFFUSEMATERIALSOURCE", 145},
                                                 {"D3DRS_SPECULARMATERIALSOURCE", 146},
                                                 {"D3DRS_AMBIENTMATERIALSOURCE", 147},
                                                 {"D3DRS_EMISSIVEMATERIALSOURCE", 148},
                                                 {"D3DRS_VERTEXBLEND", 151},
                                                 {"D3DRS_CLIPPLANEENABLE", 152},
                                                 {"D3DRS_POINTSIZE", 154},
                                                 {"D3DRS_POINTSIZE_MIN", 155},
                                                 {"D3DRS_POINTSPRITEENABLE", 156},
                                                 {"D3DRS_POINTSCALEENABLE", 157},
                                                 {"D3DRS_POINTSCALE_A", 158},
                                                 {"D3DRS_POINTSCALE_B", 159},
                                                 {"D3DRS_POINTSCALE_C", 160},
                                                 {"D3DRS_MULTISAMPLEANTIALIAS", 161},
                                                 {"D3DRS_MULTISAMPLEMASK", 162},
                                                 {"D3DRS_PATCHEDGESTYLE", 163},
                                                 {"D3DRS_DEBUGMONITORTOKEN", 165},
                                                 {"D3DRS_POINTSIZE_MAX", 166},
                                                 {"D3DRS_INDEXEDVERTEXBLENDENABLE", 167},
                                                 {"D3DRS_COLORWRITEENABLE", 168},
                                                 {"D3DRS_TWEENFACTOR", 170},
                                                 {"D3DRS_BLENDOP", 171},
                                                 {"D3DRS_POSITIONDEGREE", 172},
                                                 {"D3DRS_NORMALDEGREE", 173},
                                                 {"D3DRS_SCISSORTESTENABLE", 174},
                                                 {"D3DRS_SLOPESCALEDEPTHBIAS", 175},
                                                 {"D3DRS_ANTIALIASEDLINEENABLE", 176},
                                                 {"D3DRS_MINTESSELLATIONLEVEL", 178},
                                                 {"D3DRS_MAXTESSELLATIONLEVEL", 179},
                                                 {"D3DRS_ADAPTIVETESS_X", 180},
                                                 {"D3DRS_ADAPTIVETESS_Y", 181},
                                                 {"D3DRS_ADAPTIVETESS_Z", 182},
                                                 {"D3DRS_ADAPTIVETESS_W", 183},
                                                 {"D3DRS_ENABLEADAPTIVETESSELLATION", 184},
                                                 {"D3DRS_TWOSIDEDSTENCILMODE", 185},
                                                 {"D3DRS_CCW_STENCILFAIL", 186},
                                                 {"D3DRS_CCW_STENCILZFAIL", 187},
                                                 {"D3DRS_CCW_STENCILPASS", 188},
                                                 {"D3DRS_CCW_STENCILFUNC", 189},
                                                 {"D3DRS_COLORWRITEENABLE1", 190},
                                                 {"D3DRS_COLORWRITEENABLE2", 191},
                                                 {"D3DRS_COLORWRITEENABLE3", 192},
                                                 {"D3DRS_BLENDFACTOR", 193},
                                                 {"D3DRS_SRGBWRITEENABLE", 194},
                                                 {"D3DRS_DEPTHBIAS", 195},
                                                 {"D3DRS_WRAP8", 198},
                                                 {"D3DRS_WRAP9", 199},
                                                 {"D3DRS_WRAP10", 200},
                                                 {"D3DRS_WRAP11", 201},
                                                 {"D3DRS_WRAP12", 202},
                                                 {"D3DRS_WRAP13", 203},
                                                 {"D3DRS_WRAP14", 204},
                                                 {"D3DRS_WRAP15", 205},
                                                 {"D3DRS_SEPARATEALPHABLENDENABLE", 206},
                                                 {"D3DRS_SRCBLENDALPHA", 207},
                                                 {"D3DRS_DESTBLENDALPHA", 208},
                                                 {"D3DRS_BLENDOPALPHA", 209}};

int is_in_map(std::map<const char*, int> map, const char* name)
{
    auto it = map.find(name);

    if(it != map.end()) {
        return it->second;
    }

    return 0;
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

        if(!id) {
            PD2HOOK_LOG_ERROR("[XML] Sampler var missing id!");
            continue;
        }

        int id_int = 0;

        try {
            id_int = std::stoi(id);
        } catch(...) {
            id_int = is_in_map(D3DSAMPLERSTATETYPE, id);
        }

        if(!id_int) {
            PD2HOOK_LOG_ERROR("[XML] Sampler var invalid id!");
            continue;
        }

		if (!value)
		{
			PD2HOOK_LOG_ERROR("[XML] Sampler var missing value!");
			continue;
		}

		patch->vars[id_int] = std::stoi(value);
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

            if(!id) {
                PD2HOOK_LOG_ERROR("[XML] Render state var missing id!");
                continue;
            }

            int id_int = 0;

            try {
                id_int = std::stoi(id);
            } catch(...) {
                // TODO: Figure out why this isn't working
                id_int = is_in_map(D3DRENDERSTATETYPE, id);
            }

            if(!id_int) {
                PD2HOOK_LOG_ERROR("[XML] Render state var invalid id!");
                continue;
            }

			if (!value)
			{
				PD2HOOK_LOG_ERROR("[XML] Render state var missing value!");
				continue;
			}

			patch->render_states[id_int] = std::stoi(value);
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