#include "../includes/sampleShader.h"

SampleShader::SampleShader(LPDIRECT3DDEVICE9 dev) {
	Shader::mDev = dev;
	Shader::loadEffectFile("./shader/shadow.fx");
}

SampleShader::~SampleShader() {

}


