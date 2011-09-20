#ifdef GL_ES
precision highp float;
#endif

uniform vec2 resolution;
uniform float time;

void main(void)
{
	vec4 leftColor = vec4(21.0/255.0, 158.0/255.0, 189.0/255.0, 1.0);
	vec4 rightColor = vec4(189.0/255.0, 21.0/255.0, 189.0/255.0, 1.0);
	vec4 difference = rightColor - leftColor;
	vec2 radialGradientCenter = resolution.xy / 2.0;
	radialGradientCenter.x -= sin((time + cos(time*0.3) - (0.5*time)) / 2.0) + cos((time - sin(time / 2.0)) / 2.0) * (resolution.x * 0.8);
	radialGradientCenter.y -= cos((time + cos(time*0.83) - (0.3*time)) / 2.0) + sin((time - sin(time / 4.0)) / 2.0) * (resolution.y * 0.8);

	float redChange = difference.x / resolution.x;
	float greenChange = difference.y / resolution.x;
	float blueChange = difference.z / resolution.x;

	vec2 vectorFromCenter = radialGradientCenter - gl_FragCoord.xy;
	float distanceFromCenter = sqrt((vectorFromCenter.x * vectorFromCenter.x) + (vectorFromCenter.y * vectorFromCenter.y));
	float brightFactor = (distanceFromCenter - 300.0) / (resolution.x + resolution.y);

	float red = (leftColor.x + (gl_FragCoord.x * redChange) - brightFactor);
	float green = (leftColor.y + (gl_FragCoord.x * greenChange) - brightFactor);
	float blue = (leftColor.z + (gl_FragCoord.x * blueChange) - brightFactor);

	if(mod(gl_FragCoord.x, 3.0) <= 1.0 || mod(gl_FragCoord.y, 3.0) <= 1.0){
		red -= 0.10;
		green -= 0.10;
		blue -= 0.10;
	}

    gl_FragColor = vec4(red, green, blue, 1.0);
}

