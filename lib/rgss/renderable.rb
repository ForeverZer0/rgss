
module RGSS

  class TextShader < Graphics::Shader

    def initialize
      vert = File.read(File.join(__dir__, 'shaders', 'sprite-vert.glsl'))
      frag = File.read(File.join(__dir__, 'shaders', 'font-frag.glsl'))
      super(vert, frag)
      @model_id = locate('model')
      @color_id = locate('color')
    end

    def configure(renderable)
      use
      glUniformMatrix4fv(@model_id, 1, false, renderable.model)
      glUniform4fv(@color_id, 1, renderable.color)
    end

  end

  class SpriteShader < Graphics::Shader

    def initialize
      vert = File.read(File.join(__dir__, 'shaders', 'sprite-vert.glsl'))
      frag = File.read(File.join(__dir__, 'shaders', 'sprite-frag.glsl'))
      super(vert, frag)
      @model_id = locate('model')
      @hue_id = locate('hue')
      @color_id = locate('color')
      @tone_id = locate('tone')
      @flash_id = locate('flash')
      @opacity_id = locate('opacity')
    end

    def configure(renderable)
      use
      glUniformMatrix4fv(@model_id, 1, false, renderable.model)
      glUniform4fv(@color_id, 1, renderable.color)
      glUniform4fv(@tone_id, 1, renderable.tone)
      glUniform4fv(@flash_id, 1, renderable.flash_color)
      glUniform1f(@hue_id, renderable.hue)
      glUniform1f(@opacity_id, renderable.opacity)
    end

    def model(value)
      glUniformMatrix4fv(@model_id, 1, false, value)
    end

    def color(color)
      glUniform4fv(@color_id, 1, color)
    end

    def tone(tone)
      glUniform4fv(@tone_id, 1, tone)
    end

    def flash_color(color)
      glUniform4fv(@flash_id, 1, color)
    end

    def hue(degrees)
      glUniform1f(@hue_id, degrees)
    end

    def opacity(alpha)
      glUniform1f(@opacity_id, alpha)
    end
  end

  class Renderable < Entity

    include GL

    def self.shader
      @shader ||= SpriteShader.new
    end

    def render(delta)

      glBlendEquation(self.blend.op)
      glBlendFunc(self.blend.src, self.blend.dst)

      Renderable.shader.configure(self)
    end
  end
end