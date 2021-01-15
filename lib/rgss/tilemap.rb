require 'ox'

module RGSS

  module TMX

    class MapObject

      attr_reader :properties

      def initialize(node)

      end

      protected

      def parse_color(value)
        return Color::NONE unless value
        c = value.sub(/^#/, '').scan(/../).map { |a| a.to_i(16) / 255.0 }
        Color.new(c[1], c[2], c[3], c[0])
      end
    end


    class Map < MapObject

      def self.parse(xml)

      end

      def self.load(path)
        opts = Ox.default_options
        opts[:symbolize_keys] = true
        doc = Ox.load_file(path, opts)
        new(doc.root)
      end

      attr_reader :version
      attr_reader :orientation
      attr_reader :render_order
      attr_reader :width
      attr_reader :height
      attr_reader :tile_width
      attr_reader :tile_width
      attr_reader :side_length
      attr_reader :stagger_axis
      attr_reader :back_color

      def initialize(node)
        super(node)

        @version = node[:version]
        @orientation = node[:orientation].to_sym
        @render_order = node[:renderorder].sub('-', '').to_sym
        @width = node[:width].to_i
        @height = node[:height].to_i
        @tile_width = node[:tilewidth].to_i
        @tile_height = node[:tileheight].to_i
        @side_length = node[:hexsidelength].to_i
        @stagger_axis = node[:staggeraxis].to_i
        @infinite = !!node[:infinite]
        @back_color = parse_color(node[:backgroundcolor])

        @tilesets = node.locate('./tileset').map { |child| Tileset.new(child) }
        @layers = []
      end

      def infinite?
        @infinite
      end

      def size
        Size.new(@width, @height)
      end

      def tile_size
        Size.new(@tile_width, @tile_height)
      end

      def pixel_width
        @width * @tile_width
      end

      def pixel_height
        @height * @tile_height
      end



      private_class_method :new

    end

    class Tileset < MapObject

      def self.parse(xml)

      end

      def self.load(path)

      end

      def initialize(node)
        super(node)

      end
    end

  end
end

map = RGSS::TMX::Map.load('/home/eric/Desktop/tiled-master/examples/rpg/island.tmx')
p map.version
p map.orientation
p map.render_order
