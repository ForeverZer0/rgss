
module RGSS

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