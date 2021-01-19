
module RGSS

  class TextSprite < Sprite
      
    def self.shader
      @shader ||= TextShader.new
    end

    def layout(text, markup = true)



    end

    def render(alpha)
      return unless @texture && self.visible && self.opacity > 0.0
      TextSprite.shader.configure(self)
      @texture.bind
      glBindVertexArray(@vao)
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nil)
      glBindVertexArray(GL_NONE)
    end

  end
end