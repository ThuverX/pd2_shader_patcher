sampler2D depth : register(ps_3_0, s3);
sampler2D diffuse_texture : register(ps_3_0, s0);
sampler2D filter_color_texture : register(ps_3_0, s2);

sampler2D self_illumination_texture : register(ps_3_0, s1);

float3 ref_far_plane : register(ps_3_0, c0);
float3 ref_near_plane : register(ps_3_0, c2);
float3 ref_settings : register(ps_3_0, c1);

struct PixelMain_Input
{
    float2 texcoord : TEXCOORD;
    float2 size : TEXCOORD1;
};

// float nrand(float2 n) {
//     return frac(sin(dot(n.xy, float2(12.9898f, 78.233f))) * 43758.5453f);
// }

// float2 rot2d(float2 p, float a) {
//     float2 sc = float2(sin(a), cos(a));
//     return float2(dot(p, float2(sc.y, -sc.x)), dot(p, sc.xy));
// }

// static const float ContrastAmount = 150.0f;
// static const float3 ContrastFactor = float3(9.0f, 9.0f, 9.0f);
// static const float Smooth = 2.0f;

// float3 bokeh(float2 uv, float radius, float2 size) {

//     if(radius < 0.0001f) {
//         return tex2D(diffuse_texture, uv).rgb;
//     }

//     float3 num = 0.0;
//     float3 weight = 0.0;
    
//     float2 aspect = float2(size.y / size.x, 1.0f);
//     float pixelSize = 1.0f / size.x;
    
//     for (float i = 0.0f; i < SampleCount; i++) {
//         float2 offset = Samples[i] * radius * pixelSize;
        
//         float2 sampleUV = uv + aspect * offset;
        
//         float3 col = tex2D(diffuse_texture, sampleUV).rgb;
        
//         float3 bokeh = Smooth + pow(col, ContrastFactor) * ContrastAmount;
        
//         num += col * bokeh;
//         weight += bokeh;
//     }
    
//     return num / weight;
// }



// // find a way to do this without branching
// float testDepth(float normalizedDepth) {
//     float nearDepth = ref_near_plane.x / ref_far_plane.y;
//     float farDepth = ref_far_plane.x / ref_far_plane.y;

//     if(normalizedDepth < nearDepth) {
//         return lerp(1.0f, 0.0f, normalizedDepth / nearDepth);
//     } else if(normalizedDepth < farDepth) {
//         return 0.0f;
//     } else {
//         return lerp(0.0f, 1.0f, (normalizedDepth - farDepth) / (1.0f - farDepth));
//     }
// }

// float4 PixelMain(PixelMain_Input i) : COLOR
// {
//     float2 uv = i.texcoord.xy;

//     float distance = tex2D(depth, uv).x;

//     float normalizedDepth = saturate(distance / ref_far_plane.y);

//     float blurStrength = testDepth(normalizedDepth);

//     float s = ref_settings.z * blurStrength;

//     float3 color = bokeh(uv, s, i.size.xy);

//     return float4(color, 1.0f);
// }


static const float2 Samples[165] = {
    float2(0.73428070, 0.73380108),
	float2(0.74627946, 0.68897364),
	float2(0.77322797, 0.74212208),
	float2(0.69793431, 0.67644434),
	float2(0.67526066, 0.73935617),
	float2(0.79088539, 0.69671321),
	float2(0.71729323, 0.77312753),
	float2(0.65120343, 0.67986912),
	float2(0.62405727, 0.78636357),
	float2(0.63695375, 0.72543919),
	float2(0.60647324, 0.75138525),
	float2(0.58370063, 0.65683492),
	float2(0.60100065, 0.70724755),
	float2(0.66359112, 0.61089754),
	float2(0.63372542, 0.64362119),
	float2(0.54860271, 0.67746496),
	float2(0.56524341, 0.74262199),
	float2(0.76454533, 0.64250683),
	float2(0.50572631, 0.68746099),
	float2(0.54516612, 0.60795947),
	float2(0.51112386, 0.74562887),
	float2(0.55117736, 0.80777205),
	float2(0.80326437, 0.65628133),
	float2(0.72284992, 0.64027362),
	float2(0.77019778, 0.59373496),
	float2(0.72814634, 0.59065110),
	float2(0.82601252, 0.60364511),
	float2(0.80394920, 0.56380764),
	float2(0.82688743, 0.52383142),
	float2(0.77254120, 0.50665315),
	float2(0.74817640, 0.54698645),
	float2(0.46308440, 0.74652572),
	float2(0.50352421, 0.64721394),
	float2(0.47050684, 0.66891293),
	float2(0.47015791, 0.62297044),
	float2(0.42960239, 0.70369797),
	float2(0.42010215, 0.61135195),
	float2(0.39462147, 0.65743082),
	float2(0.38185089, 0.55268980),
	float2(0.34966309, 0.60800817),
	float2(0.47632347, 0.56817454),
	float2(0.42121139, 0.54939422),
	float2(0.42998513, 0.79683721),
	float2(0.50350398, 0.79238716),
	float2(0.39548241, 0.76152590),
	float2(0.35909320, 0.79511819),
	float2(0.46323861, 0.83707578),
	float2(0.52030214, 0.56815619),
	float2(0.37529982, 0.70835588),
	float2(0.76807043, 0.46702301),
	float2(0.72963235, 0.49765262),
	float2(0.82081726, 0.46916865),
	float2(0.70734707, 0.54631261),
	float2(0.47205939, 0.51397866),
	float2(0.39707387, 0.47605229),
	float2(0.51616409, 0.83160975),
	float2(0.35520607, 0.49931742),
	float2(0.32308469, 0.52481931),
	float2(0.47880588, 0.46504225),
	float2(0.44403968, 0.44631279),
	float2(0.52572094, 0.46742999),
	float2(0.52459649, 0.52596773),
	float2(0.28559584, 0.63567762),
	float2(0.32870582, 0.64595863),
	float2(0.31067852, 0.58922113),
	float2(0.66848484, 0.46972416),
	float2(0.68979447, 0.50611379),
	float2(0.70428121, 0.44363412),
	float2(0.59427361, 0.61202541),
	float2(0.62405227, 0.58419356),
	float2(0.51507580, 0.87787981),
	float2(0.52666036, 0.40582375),
	float2(0.56548632, 0.50239814),
	float2(0.58097628, 0.41690094),
	float2(0.56763267, 0.46090105),
	float2(0.73708937, 0.41178306),
	float2(0.82778254, 0.42958241),
	float2(0.78735776, 0.42106047),
	float2(0.55820814, 0.54959973),
	float2(0.64074442, 0.51430978),
	float2(0.76866171, 0.36721116),
	float2(0.73183680, 0.35210191),
	float2(0.68659438, 0.36529011),
	float2(0.68293271, 0.41080505),
	float2(0.27670850, 0.51523106),
	float2(0.26823680, 0.56416824),
	float2(0.31577844, 0.46346400),
	float2(0.82520028, 0.38153978),
	float2(0.69278629, 0.32431684),
	float2(0.72290484, 0.29760262),
	float2(0.62865601, 0.35538780),
	float2(0.62972510, 0.40285730),
	float2(0.62548035, 0.47255145),
	float2(0.60221279, 0.54170344),
	float2(0.32495404, 0.68683681),
	float2(0.33081173, 0.75000206),
	float2(0.65616710, 0.55454760),
	float2(0.56164862, 0.38025533),
	float2(0.34321417, 0.56753512),
	float2(0.59213029, 0.81190651),
	float2(0.56634459, 0.84743916),
	float2(0.39841746, 0.82790817),
	float2(0.62078424, 0.27928709),
	float2(0.58534588, 0.30590289),
	float2(0.56030137, 0.33736590),
	float2(0.65358932, 0.31424151),
	float2(0.27431251, 0.70394720),
	float2(0.22142674, 0.60159419),
	float2(0.24540495, 0.66822197),
	float2(0.25497820, 0.47487119),
	float2(0.23497547, 0.51665602),
	float2(0.21470360, 0.47971805),
	float2(0.19883551, 0.71753078),
	float2(0.20179441, 0.55401417),
	float2(0.70988012, 0.25328468),
	float2(0.76258035, 0.31004726),
	float2(0.66296791, 0.26890759),
	float2(0.16782779, 0.48360402),
	float2(0.21170268, 0.43466724),
	float2(0.19903587, 0.64432324),
	float2(0.17818096, 0.59786001),
	float2(0.80668689, 0.31968744),
	float2(0.54841617, 0.24148280),
	float2(0.51212066, 0.33210452),
	float2(0.51233584, 0.28837799),
	float2(0.16835793, 0.44183338),
	float2(0.48769352, 0.42456864),
	float2(0.46300638, 0.38702216),
	float2(0.27425831, 0.39112116),
	float2(0.22572406, 0.38689325),
	float2(0.17434024, 0.38152077),
	float2(0.46112265, 0.29842267),
	float2(0.44392356, 0.35175223),
	float2(0.30478588, 0.42468925),
	float2(0.35786969, 0.42648649),
	float2(0.39473046, 0.40586733),
	float2(0.17662617, 0.67717975),
	float2(0.36230333, 0.37543719),
	float2(0.32211490, 0.37450351),
	float2(0.26524461, 0.32780995),
	float2(0.25058894, 0.42912454),
	float2(0.20404858, 0.33494390),
	float2(0.47562632, 0.23426082),
	float2(0.58249555, 0.18901781),
	float2(0.59673113, 0.22810154),
	float2(0.53926901, 0.20297851),
	float2(0.55195464, 0.28548342),
	float2(0.79515403, 0.28224581),
	float2(0.29704535, 0.29840793),
	float2(0.24612283, 0.27868966),
	float2(0.65421045, 0.20290757),
	float2(0.33232973, 0.32194157),
	float2(0.33439221, 0.25010702),
	float2(0.37399553, 0.29551515),
	float2(0.39018127, 0.33874545),
	float2(0.51961886, 0.16662263),
	float2(0.47508418, 0.18870344),
	float2(0.42074381, 0.30554956),
	float2(0.43297226, 0.23070332),
	float2(0.38708568, 0.24236710),
	float2(0.44087218, 0.16602513),
	float2(0.48325117, 0.14106281),
	float2(0.37643763, 0.20318945),
	float2(0.30424110, 0.22273596),
	float2(0.27824477, 0.25279121)
};

static const float SampleCount = 16.0f;

static const float ContrastAmount = 150.0f;
static const float3 ContrastFactor = float3(9.0f, 9.0f, 9.0f);
static const float Smooth = 2.0f;

// float3 blurWithMask(float2 uv, float radius, float2 screenSize, float mask) {
//     if(radius < 0.0001f) {
//         return tex2D(diffuse_texture, uv).rgb;
//     }

//     float3 num = 0.0;
//     float3 weight = 0.0;
    
//     float2 aspect = float2(screenSize.y / screenSize.x, 1.0f);
//     float pixelSize = 1.0f / screenSize.x;
    
//     for (float i = 0.0f; i < SampleCount; i++) {
//         float2 offset = Samples[i] * radius * pixelSize;
        
//         float2 sampleUV = uv + aspect * offset;
        
//         float3 col = tex2D(diffuse_texture, sampleUV).rgb;
        
//         float3 bokeh = Smooth + pow(col, ContrastFactor) * ContrastAmount;
        
//         num += col * bokeh * mask;
//         weight += bokeh;
//     }
    
//     return num / weight;
// }

texture2D AFX_CoC { Width = BUFFER_WIDTH; Height = BUFFER_HEIGHT; Format = RG8; };
sampler2D CoC { Texture = AFX_CoC; MagFilter = POINT; MinFilter = POINT; MipFilter = POINT;};

float3 nearBlur(float2 uv, float radius, float2 screenSize, float depth) {
    // return saturate(blurWithMask(uv, radius, screenSize, depth));
    return float3(1.0f, 0.0f, 0.0f);
}

float3 farBlur(float2 uv, float radius, float2 screenSize, float depth) {
    return float3(0.0f, 1.0f, 0.0f);
    // return saturate(blurWithMask(uv, radius, screenSize, depth));
}

float3 centerBlur(float2 uv, float nearDepth, float farDepth) {
    float mask = nearDepth - farDepth;

    return saturate(tex2D(diffuse_texture, uv).rgb * mask);
}


float3 getCoC(float2 uv)
{
    float normalizedDepth = tex2D(depth, uv).x / ref_far_plane.y;
    float nearDepth = ref_near_plane.x / ref_far_plane.y;
    float farDepth = ref_far_plane.x / ref_far_plane.y;

    if(normalizedDepth < nearDepth) {
        return float3(lerp(1.0f, 0.0f, normalizedDepth / nearDepth), 0, 0);
    } else if(normalizedDepth < farDepth) {
        return float3(0, 0, 0);
    } else {
        return float3(0,lerp(0.0f, 1.0f, (normalizedDepth - farDepth) / (1.0f - farDepth)), 0);
    }
}

float4 Main(PixelMain_Input input) : COLOR
{
    float2 uv = input.texcoord.xy;

    // float3 CoC = getCoC(uv);

    // // float radius = ref_settings.z;

    // // float3 nearColor = nearBlur(uv, radius, input.size.xy, CoC.x);
    // // float3 centerColor = centerBlur(uv, CoC.x, CoC.y);
    // // float3 farColor = farBlur(uv, radius, input.size.xy, CoC.y);

    float3 color = float3(uv, 0.0f);

    return float4(color, 1.0f);
}