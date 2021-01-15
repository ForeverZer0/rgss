
module RGSS

  ##
  # @abstract 
  # Abstract base class for game objects that represent a native type.
  class GameObject

    attr_accessor :id

    def initialize(id)
      @id = id
    end

    def ==(other)
      self.class == other.class && @id == other.id
    end

    def valid?
      raise(NotImplementedError, "#{__method__} must be overriden in derived classes")
    end

    def dispose
      @id = 0
      nil
    end
  end
end