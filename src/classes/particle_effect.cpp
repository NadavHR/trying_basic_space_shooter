#include "../headers/particle_effect.hpp"


ParticleEffect::ParticleEffect(Shader * shader, float duration, size_t count, Model &model) :
    TimedEffect(shader, duration), mmodel(model) {
    mcount = count;
}


void ParticleEffect::draw() {
    for (auto mesh : mmodel.meshes) {
        mesh.bindTextures(*mShader);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(mShader->getProgramID(), "frame"), 0);
        glBindTexture(GL_TEXTURE_2D, rendering::renderer->getTexture());
        // draw mesh
        glBindVertexArray(mesh.VAO);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(mesh.indices.size()), GL_UNSIGNED_INT, 0, mcount);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }
}