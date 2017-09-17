#extern version.glsl

layout(points) in; 
layout(triangle_strip, max_vertices = 4) out; 

uniform vec3 position;
uniform vec3 scale;
uniform int cols;
uniform int lines;
uniform int maxhealth;
uniform int health;

uniform mat4 mvp_matrix;
uniform vec3 camera_pos;

//out to fragment shader
out vec2 uv1;
out vec2 uv2;
out float blendf;
out float fade;

void processVertex(vec3 up, vec3 right, vec2 tex1, vec2 tex2, float sizex, float sizey, float offx, float offy)
{
	vec4 vertex_pos = vec4(position + (up + right) * scale, 1.0);
	gl_Position = mvp_matrix * vertex_pos;
	uv1 = vec2(tex1.x * sizex + offx, tex1.y * sizey + offy);
	uv2 = vec2(tex2.x * sizex + offx, tex2.y * sizey + offy);
	EmitVertex();
}

vec2 genUV(int spriteID)
{
	if (spriteID < 0)
	{
		spriteID = 0;
	}
	//texture atlas calculation
	float texx = spriteID % cols;	// x texture to use in texture atlases
	float texy = spriteID / lines;	// y texture to use in texture atlases
	return vec2(texx, texy);
}


void main(void)
{
	int maxID = cols * lines;
	float ratio = 1 - health / float(maxhealth);
	float fspriteID = ratio * maxID;
	int spriteID1 = int(fspriteID);
	int spriteID2 = spriteID1 == maxID - 1 ? spriteID1 : spriteID1 + 1;
	
	blendf = mod(fspriteID, 1.0f);
	
	if (spriteID1 == maxID - 1)
	{
		fade = (1 - (fspriteID - spriteID1));
	}
	else
	{
		fade = 1;
	}
	
	float textunitx = 1.0 / cols;
	float textunity = 1.0 / lines;
	
	vec3 direction = normalize(camera_pos - position);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 vec_up = cross(up, direction);
	vec3 vec_right = cross(direction, vec_up);
	
	vec2 uv1 = genUV(spriteID1);
	vec2 uv2 = genUV(spriteID2);
	
	processVertex(-vec_up, -vec_right, uv1, uv2, textunitx, textunity, 0, textunity);
	processVertex(-vec_up, vec_right, uv1, uv2, textunitx, textunity, 0, 0);
	processVertex(vec_up, -vec_right, uv1, uv2, textunitx, textunity, textunitx, textunity);
	processVertex(vec_up, vec_right, uv1, uv2, textunitx, textunity, textunitx, 0);


}
