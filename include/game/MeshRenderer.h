#pragma once

#include <math/Transform.h>

#include <graphics/Mesh.h>
#include <graphics/Shader.h>
#include <graphics/Material.h>

#include <game/GameComponent.h>

class MeshRenderer : public GameComponent {
	Mesh mesh;
	Material material;

    public:
	MeshRenderer() = delete;
	MeshRenderer(const Mesh &mesh, const Material &material);

	void input(const Transform &transform) override;
	void update(const Transform &transform) override;
	void render(const Transform &transform, Shader &shader) override;
};