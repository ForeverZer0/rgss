$RGSS_DEBUG = true
p ENV['SDL_GAMECONTROLLERCONFIG']

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
      $e.y -= SPEED if Input.press?(:UP)
      $e.y += SPEED if Input.press?(:DOWN)
      $e.x -= SPEED if Input.press?(:LEFT)
      $e.x += SPEED if Input.press?(:RIGHT)
      $e.z += 1 if Input.press?(:RAISE)
      $e.z -= 1 if Input.press?(:LOWER)

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

  Game.create(1024, 768, "RGSS v.#{VERSION} [Dev]", resizable: true, vsync: true)
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

  # fog_tex = Texture.load("/storage/images/RTP/XP/Graphics/Fogs/001-Fog01.png")
  # # $v.angle = 23
  # $fog = Plane.new
  # $fog.texture = fog_tex
  # $fog.size = Size.new(1024, 768)
  # $fog.scroll = vec2(320.0, 1240.0)
  # $fog.opacity = 0.5
  # $fog.visible = false

  markup = <<-EOS
regular <b>bold</b> <i>italic</i> <u>underline</u> <span overline="single">overline</span>
<s>strikethrough</s> <tt>monospace</tt> <b><u>bold underline</u></b> 
<span font_style="oblique">oblique</span> <small>smaller</small> <big>bigger</big> <sup>superscript</sup> <sub>subscript</sub>
<span font="Comic Sans MS">Mid-string font change</span>
<span foreground=\"#FFCC00\"> colored</span>
EOS



  $e = Emitter.new(20000, nil) do |e|
    # e.texture = Texture.load("/home/eric/Desktop/smoke.png")
    e.size = Size.new(1, 1)
    e.position = vec2(1024 / 2, 768 / 2)
    # e.velocity = vec3(3.0, -10.0, 1.0)
    e.radius = 64
    # e.friction = 3
    e.rate = 30
    e.gravity = 10
    e.force = 120
    e.angular_velocity = vec2(5)
    # e.gravity = Emitter::EARTH_GRAVITY * 32
    e.wind = 40
    e.fade = 50
    e.lifespan = 30 * 8
    # e.zoom = 5.5
    # e.fade = 5.5
    # e.spectrum = [Color::YELLOW, Color::ORANGE, Color::RED, Color::WHITE]
    # e.scale = vec3(0.1, 0.1, 0)
  end





  Game.speed = 1.0
  begin
    Game.main(30)
  rescue
    Log.fatal($!.to_s)
  end
  Game.terminate
end
