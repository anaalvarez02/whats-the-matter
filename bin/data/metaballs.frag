#version 150

out vec4 fragColor;

uniform float mSizes[20];
uniform vec2 mPositions[20];

uniform vec4 innerColor;
uniform vec4 outerColor;

float calculateMetaballColour(vec2 mPos, float mRadius) {
    
    vec2 pixelPos = gl_FragCoord.xy;
    
    float cDist = distance(pixelPos, mPos);
    
    float normDist = mRadius / cDist;
    
    return normDist;
    
}

void main() {
    
    float metaballAlpha = 0;
    
    for (int i = 0; i < 20; i++)
        metaballAlpha += calculateMetaballColour(mPositions[i], mSizes[i]);
    
    vec4 metaballRGB = mix(outerColor, innerColor, metaballAlpha);
    
    fragColor = vec4(metaballRGB.xyz, metaballAlpha);
    
}
