
module RGSS

  class Sprite < Renderable

    FLIP_NONE = 0x00
    FLIP_X    = 0x01
    FLIP_Y    = 0x02
    FLIP_BOTH = FLIP_X | FLIP_Y

    attr_reader   :texture
    attr_accessor :flip

    def initialize(*args, **opts)
      super(opts[:viewport]&.batch || Graphics.batch)
      vertex_setup
      @flip = FLIP_NONE
      self.texture = nil
      self.src_rect = Rect.empty
    end

    def texture=(value)
      @texture = value
      self.src_rect = @texture ? @texture.rect : Rect.empty
      value
    end

    def flip=(value)
      @flip = value || FLIP_NONE
      update_vertices
      value
    end

    def src_rect=(rect)
      @src_rect = rect || Rect.empty
      self.size = rect.size
      update_vertices
      rect
    end

    def render(alpha)
      return unless @texture && @visible && @opacity > 0.0
      super(alpha)
      @texture.bind
      glBindVertexArray(@vao)
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nil)
      glBindVertexArray(GL_NONE)
    end


    def update_vertices
      return unless @texture

      l = @src_rect.x / @texture.width.to_f
      t = @src_rect.y / @texture.height.to_f
      r = l + @src_rect.width / @texture.width.to_f
      b = t + @src_rect.height / @texture.height.to_f

      if (@flip & FLIP_X).nonzero?
        l, r = r, l
      end
      if (@flip & FLIP_Y).nonzero?
        t, b = b, t
      end
      vertices = 
      [
        0.0, 1.0, l, b, # Bottom-Left
        1.0, 0.0, r, t, # Top-Right
        0.0, 0.0, l, t, # Top-Left
        1.0, 1.0, r, b, # Bottom-Right
      ].pack('f*')
      glBindBuffer(GL_ARRAY_BUFFER, @vbo)
      glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size, vertices)
      glBindBuffer(GL_ARRAY_BUFFER, 0)
    end
  end


  class SpriteAtlas < Sprite

    attr_reader :region
    attr_reader :columns
    attr_reader :rows

    attr_reader :cx
    attr_reader :cy

    def initialize(*args, **opts)
      @region = opts[:region]
      @columns = opts[:columns] || 0
      @rows = opts[:rows] || 0
      super

      @cx = 0
      @cy = 0
      update_src_rect
    end

    def cx=(cell_x)
      @cx = @columns.zero? ? 0 : cell_x % @columns
      update_src_rect
      cell_x
    end

    def cy=(cell_y)
      @cy = @rows.zero? ? 0 : cell_y % @rows
      update_src_rect
      cell_y
    end

    def region=(value)
      @region = value 
      update_src_rect
      value
    end

    def select(cell_x, cell_y)
      @cx = @columns.zero? ? 0 : cell_x % @columns
      @cy = @rows.zero? ? 0 : cell_y % @rows
      update_src_rect
    end

    private

    def update_src_rect
      return unless @texture

      @region ||= @texture.rect
      @cw = @columns.zero? ? 0 : @region.width / @columns
      @ch = @rows.zero? ? 0 : @region.height / @rows

      cell_x = (@cx * @cw) + @region.x
      cell_y = (@cy * @ch) + @region.y

      self.src_rect = Rect.new(cell_x, cell_y, @cw, @ch)
    end
  end
  
end