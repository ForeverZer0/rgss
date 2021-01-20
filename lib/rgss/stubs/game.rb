module RGSS

  module Game

    class << self

      attr_accessor :title
      attr_accessor :fullscreen
    end

    def self.init
    end

    def self.create(width, height, title, **opts)
    end

    def self.center
    end

    def self.close(flag = true)
    end

    def self.closing?
    end

    def self.terminate
    end
  end

end