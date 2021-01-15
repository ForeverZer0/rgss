module RGSS

  class Batch

    include Enumerable

    def initialize
      @invalid = false
      @items = Array.new
    end

    def each
      return enum_for(__method__) unless block_given?
      if @invalid
        @items.sort! { |a, b| a.z <=> b.z }
        @invalid = false
      end
      @items.each { |item| yield item }
    end

    def add(object)
      unless object.respond_to?(:z) && object.respond_to?(:render)
        raise(TypeError, "#{object} is not a #{Renderable} instance")
      end
      return false if @items.include?(object)
      @items.push(object)
      @invalid = true
      true
    end

    def remove(object)
      @items.delete(object)
    end

    def contains?(object)
      object.is_a?(Renderable) ? @items.include?(object) : false
    end

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
      attr_reader :batch
    end
  
    @batch = Batch.new
  end
end
