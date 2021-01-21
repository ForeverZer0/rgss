$RGSS_DEBUG = true

module RGSS
  require "logger"

  Log = Logger.new(STDOUT)

  Log.formatter = proc do |severity, datetime, progname, msg|
    time = datetime.strftime("%H:%M:%S")
    prefix = case severity
      when "DEBUG" then "[\33[1;35mDEBUG\33[0m]"
      when "INFO" then "[\33[1;32mINFO\33[0m] "
      when "WARN" then "[\33[1;33mWARN\33[0m] "
      when "ERROR" then "[\33[1;31mERROR\33[0m]"
      when "FATAL" then "[\33[1;31mFATAL\33[0m]"
      else "LOG"
      end
    "#{time} #{prefix} #{msg}\n"
  end
end

require_relative "rgss/version"
require_relative "rgss/rgss"

module RGSS
  module Game
    SPEED = 8

    def self.update(delta)
      $sprite.y -= SPEED if Input.press?(:UP)
      $sprite.y += SPEED if Input.press?(:DOWN)
      $sprite.x -= SPEED if Input.press?(:LEFT)
      $sprite.x += SPEED if Input.press?(:RIGHT)
      $sprite.z += 1 if Input.press?(:RAISE)
      $sprite.z -= 1 if Input.press?(:LOWER)

      s = $fog.size
      if Input.press?(:RAISE)
        $fog.size = Size.new(s.width + 16, s.height + 16)
      end
      if Input.press?(:LOWER)
        $fog.size = Size.new(s.width - 16, s.height - 16)
      end

      $fog.position = vec2((1024 - s.width) * 0.5, (768 - s.height) * 0.5)

      $sprite.update(delta)
      $v.update(delta)
      $fog.update(delta)
      $e.update(delta)

      if Input.trigger?(:FULLSCREEN)
        p Graphics.fps
        p $e.scale
        p $e.position
      end

      if Input.press?(:QUIT)
        Game.close(true)
      end
    end
  end

  $RGSS_DEBUG = true
  Game.create(1024, 768, "RGSS", resizable: true, vsync: true)
  # Graphics.back_color = Color::DARK_SLATE_GRAY
  

  Game.icon = "/storage/images/anvil.png"

  Input.bind(:UP, [Input::KEY_W, Input::KEY_UP])
  Input.bind(:LEFT, [Input::KEY_A, Input::KEY_LEFT])
  Input.bind(:DOWN, [Input::KEY_S, Input::KEY_DOWN])
  Input.bind(:RIGHT, [Input::KEY_D, Input::KEY_RIGHT])
  Input.bind(:RAISE, Input::KEY_Q)
  Input.bind(:LOWER, Input::KEY_E)
  Input.bind(:CONFIRM, Input::KEY_SPACE)
  Input.bind(:FULLSCREEN, Input::KEY_F)
  Input.bind(:QUIT, Input::KEY_ESCAPE)

  Input.cursor_image("/home/eric/Desktop/cursor.png")
  $v = Viewport.new(1, 1, 256, 256)
  $v.back_color = Color::BLACK
  $v.position = vec2(400, 100)

  fog_tex = Texture.load("/storage/images/RTP/XP/Graphics/Fogs/001-Fog01.png")

  # $v.angle = 23
  $fog = Plane.new
  $fog.texture = fog_tex
  $fog.size = Size.new(1024, 768)
  $fog.scroll = vec2(320.0, 1240.0)
  $fog.opacity = 0.5
  $fog.visible = false

  markup = <<-EOS
regular <b>bold</b> <i>italic</i> <u>underline</u> <span overline="single">overline</span>
<s>strikethrough</s> <tt>monospace</tt> <b><u>bold underline</u></b> 
<span font_style="oblique">oblique</span> <small>smaller</small> <big>bigger</big> <sup>superscript</sup> <sub>subscript</sub>
<span font="Comic Sans MS">Mid-string font change</span>
<span foreground=\"#FFCC00\"> colored</span>
EOS

  Font.add_file("/home/eric/open_rpg/fonts/MinecraftCHMC.ttf")

  font = Font.new("MinecraftCHMC", 32)
  size = font.measure(markup)
  baked = font.bake(markup, size, align: Font::ALIGN_CENTER)

  args = Font.test_texture(markup, "MinecraftCHMC 18", Size.empty)
  $sprite = Sprite.new($v)
  $sprite.flip = 2
  $sprite.texture = Texture.wrap(*args)

  $e = Emitter.new(20000, nil)
  $e.texture = Texture.load("/home/eric/Desktop/smoke.png")
  $e.position = vec2(256, 256)
  $e.velocity = vec3(67.0, 82.0, 0.0)
  $e.scale = vec3(0.2, 0.2, 1)

  Game.speed = 1.0
  Game.main(30)
  Game.terminate
end
