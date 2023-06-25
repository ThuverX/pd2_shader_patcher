float3 render_target_size : register(vs_3_0, c4);
column_major float4x4 view_proj_matrix : register(vs_3_0, c0);
struct VertexMain_Input
{
    float4 position : POSITION;
    float4 texcoord : TEXCOORD;
};

struct VertexMain_Output
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
    float2 size : TEXCOORD1;
};

VertexMain_Output Main(VertexMain_Input i)
{
    VertexMain_Output o;

    o.position.x = dot(i.position, (view_proj_matrix._m00_m10_m20_m30));
    o.position.y = dot(i.position, (view_proj_matrix._m01_m11_m21_m31));
    o.position.w = dot(i.position, (view_proj_matrix._m03_m13_m23_m33));
    o.size = render_target_size.xy;
    o.position.z = float1(1);
    o.texcoord = i.texcoord;

    return o;
}
