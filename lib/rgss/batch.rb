module RGSS

  ##
  # @api private
  # A container for logical groupings of renderable objects. An object's depth (i.e. "z") is only relative to other
  # objects within the same batch, and are sorted thus. It is **strongly** recommended not use this class directly, and
  # use higher-level abstractions that utilize it, such as the {Viewport} and {Renderable} classes.
  class Batch

    include Enumerable

    def initialize
      @invalid = false
      @items = Array.new
    end

    ##
    # @overload each(&block)
    #   @yieldparam obj [Renderable]
    #   @return [Enumerable]
    #
    # @overload each
    #   @return [self]
    def each
      return enum_for(__method__) unless block_given?
      if @invalid
        @items.sort! { |a, b| a.z <=> b.z }
        @invalid = false
      end
      @items.each { |item| yield item }
      self
    end

    ##
    #
    # @param object [Renderable, #z, #render] an object that responds to #z and #render.
    def add(object)
      unless object.respond_to?(:z) && object.respond_to?(:render)
        raise(TypeError, "cannot add object to batch, must respond to both #z and #render")
      end
      return false if @items.include?(object)
      @items.push(object)
      @invalid = true
      true
    end

    ##
    # @param object [Object]
    def remove(object)
      @items.delete(object)
    end

    ##
    # Queries the batch to if the specified object is a child.
    #
    # @param object [Object] the object to test.
    # @return `true` is object is a child of the instance, otherwise `false`.
    def contains?(object)
      object.is_a?(Renderable) ? @items.include?(object) : false
    end

    ##
    # Marks the state of the {Batch} as invalid, indicating it should sort its children before the next enumeration.
    # Adding an object to the batch and {Renderable} instances automatically invalidate it when required, but custom
    # classes must do so manually when the objects render-order has been changed.
    #
    # @return [void]
    def invalidate
      @invalid = true
    end

    alias_method :push, :add
    alias_method :<<, :add
    alias_method :delete, :remove
    alias_method :include?, :contains?
  end

  module Graphics

    class << self

      ##
      # @return [Batch] top-level collection that is parent to all objects which will be rendered each frame.
      attr_reader :batch
    end
  
    @batch = Batch.new
  end
end
