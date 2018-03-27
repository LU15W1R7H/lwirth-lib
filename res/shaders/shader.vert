#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
    vec4 gl_Position;
};

layout(push_constant) uniform PushConstants
{
	layout(offset = 16)mat4 modelMatrix;
} pushConts;

layout(set = 1, binding = 0) uniform UBOProjection
{
	mat4 view;
	mat4 projection;
} uboProjection;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

void main() {

	vec4 worldPos = pushConts.modelMatrix * vec4(inPos, 1.0);
	gl_Position = uboProjection.projection * uboProjection.view * worldPos;
	fragColor = inColor;
}