
module RGSS

  class Blend

    ZERO                     = 0x0000
    ONE                      = 0x0001
    SRC_COLOR                = 0x0300
    ONE_MINUS_SRC_COLOR      = 0x0301
    DST_COLOR                = 0x0306
    ONE_MINUS_DST_COLOR      = 0x0307
    SRC_ALPHA                = 0x0302
    ONE_MINUS_SRC_ALPHA      = 0x0303
    DST_ALPHA                = 0x0304
    ONE_MINUS_DST_ALPHA      = 0x0305
    CONSTANT_COLOR           = 0x8001
    ONE_MINUS_CONSTANT_COLOR = 0x8002
    CONSTANT_ALPHA           = 0x8003
    ONE_MINUS_CONSTANT_ALPHA = 0x8004
    SRC_ALPHA_SATURATE       = 0x0308
    SRC1_COLOR               = 0x88F9
    ONE_MINUS_SRC1_COLOR     = 0x88FA
    SRC1_ALPHA               = 0x8589
    ONE_MINUS_SRC1_ALPHA     = 0x88FB

    OP_ADD              = 0x8006
    OP_SUBTRACT         = 0x800A
    OP_REVERSE_SUBTRACT = 0x800B
    OP_MIN              = 0x8007
    OP_MAX              = 0x8008

    attr_accessor :op
    attr_accessor :src
    attr_accessor :dst

    def initialize(op, src, dst)
      @op = op
      @src = src
      @dst = dst
    end

    def self.default
      new(OP_ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA)
    end

  end
end