#include <graphics/PhongShader.h>

#include <graphics/Shader.h>
#include <graphics/Material.h>
#include <graphics/BaseLight.h>
#include <graphics/DirectionalLight.h>
#include <graphics/PointLight.h>

#include <math/Matrix4f.h>
#include <math/Vector3f.h>

#include <core/RenderUtil.h>

#include <iostream>
#include <vector>
#include <exception>

PhongShader::PhongShader()
	: Shader()
{
}

PhongShader &PhongShader::get_instance()
{
	static PhongShader instance;
	return instance;
}

void PhongShader::load_shaders(const std::string &vertex_path,
			       const std::string &fragment_path)
{
	this->load(vertex_path, fragment_path);
	this->add_uniform("transform");
	this->add_uniform("transform_projected");
	this->add_uniform("base_color");

	this->add_uniform("ambient_light");
	this->add_uniform("directional_light.base_light.color");
	this->add_uniform("directional_light.base_light.intensity");
	this->add_uniform("directional_light.direction");

	this->add_uniform("specular.intensity");
	this->add_uniform("specular.exponent");
	this->add_uniform("eyePos");

	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		const std::string prefix = "point_lights[" + std::to_string(i);
		add_uniform(prefix + "].base_light.color");
		add_uniform(prefix + "].base_light.intensity");
		add_uniform(prefix + "].attenuation.constant");
		add_uniform(prefix + "].attenuation.linear");
		add_uniform(prefix + "].attenuation.exponent");
		add_uniform(prefix + "].position");
		add_uniform(prefix + "].range");
	}

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
		const std::string prefix = "spot_lights[" + std::to_string(i);
		add_uniform(prefix + "].point_light.base_light.color");
		add_uniform(prefix + "].point_light.base_light.intensity");
		add_uniform(prefix + "].point_light.attenuation.constant");
		add_uniform(prefix + "].point_light.attenuation.linear");
		add_uniform(prefix + "].point_light.attenuation.exponent");
		add_uniform(prefix + "].point_light.position");
		add_uniform(prefix + "].point_light.range");
		add_uniform(prefix + "].direction");
		add_uniform(prefix + "].cutoff");
	}
}

void PhongShader::update_uniforms(const Matrix4f &world_matrix,
				  const Matrix4f &projected_matrix,
				  const Material &material,
				  const Vector3f &camera_position)
{
	if (material.get_texture().get_id() > 0) {
		material.get_texture().bind();
	} else {
		RenderUtil::unbind_textures();
	}

	this->set_uniform("transform_projected", projected_matrix);
	this->set_uniform("transform", world_matrix);

	this->set_uniform("ambient_light", ambient_light);
	this->set_uniform("directional_light", directional_light);

	for (int i = 0; i < point_lights.size(); i++) {
		const std::string uniform =
			"point_lights[" + std::to_string(i) + "]";
		this->set_uniform(uniform, point_lights[i]);
	}
	for (int i = 0; i < spot_lights.size(); i++) {
		const std::string uniform =
			"spot_lights[" + std::to_string(i) + "]";
		this->set_uniform(uniform, spot_lights[i]);
	}

	this->set_uniform("base_color", material.get_color());
	this->set_uniform("specular", material.get_specular());
	this->set_uniform("eyePos", camera_position);
}

Vector3f &PhongShader::get_ambient_light() noexcept
{
	return ambient_light;
}

void PhongShader::set_ambient_light(const Vector3f &ambient_light) noexcept
{
	this->ambient_light = Vector3f(ambient_light);
}

DirectionalLight &PhongShader::get_directional_light() noexcept
{
	return directional_light;
}

void PhongShader::set_directional_light(
	const DirectionalLight &directional_light) noexcept
{
	this->directional_light.base_light = directional_light.base_light;
	this->directional_light.direction =
		directional_light.direction.normalize();
}

void PhongShader::set_point_lights(const std::vector<PointLight> &point_lights)
{
	if (point_lights.size() > MAX_POINT_LIGHTS) {
		std::cerr << "Error: Too Many Point Lights:\tCurrent: "
			  << point_lights.size()
			  << "\tMax: " << MAX_POINT_LIGHTS << '\n';
		throw std::invalid_argument("Too many Point Lights");
	}

	this->point_lights = point_lights;
}

void PhongShader::set_spot_lights(const std::vector<SpotLight> &spot_lights)
{
	if (point_lights.size() > MAX_SPOT_LIGHTS) {
		std::cerr << "Error: Too Many Spot Lights:\tCurrent: "
			  << spot_lights.size() << "\tMax: " << MAX_SPOT_LIGHTS
			  << '\n';
		throw std::invalid_argument("Too many Spot Lights");
	}

	this->spot_lights = spot_lights;
}

void PhongShader::set_uniform(const std::string &uniform,
			      const BaseLight &base_light) noexcept
{
	this->set_uniform(uniform + ".color", base_light.color);
	this->set_uniform(uniform + ".intensity", base_light.intensity);
}

void PhongShader::set_uniform(const std::string &uniform,
			      const DirectionalLight &directional_light) noexcept
{
	set_uniform(uniform + ".base_light", directional_light.base_light);
	set_uniform(uniform + ".direction", directional_light.direction);
}

void PhongShader::set_uniform(const std::string &uniform,
			      const Specular &specular) noexcept
{
	set_uniform(uniform + ".intensity", specular.intensity);
	set_uniform(uniform + ".exponent", specular.exponent);
}

void PhongShader::set_uniform(const std::string &uniform,
			      const PointLight &point_light) noexcept
{
	set_uniform(uniform + ".base_light", point_light.base_light);
	set_uniform(uniform + ".attenuation.constant",
		    point_light.attenuation.constant);
	set_uniform(uniform + ".attenuation.linear",
		    point_light.attenuation.linear);
	set_uniform(uniform + ".attenuation.exponent",
		    point_light.attenuation.exponent);
	set_uniform(uniform + ".position", point_light.position);
	set_uniform(uniform + ".range", point_light.range);
}

void PhongShader::set_uniform(const std::string &uniform,
			      const SpotLight &spot_light) noexcept
{
	set_uniform(uniform + ".point_light", spot_light.point_light);
	set_uniform(uniform + ".direction", spot_light.direction);
	set_uniform(uniform + ".cutoff", spot_light.cutoff);
}