$RGSS_DEBUG = true # TODO

require_relative 'rgss/version'
require_relative 'rgss/log'
require_relative 'rgss/rgss'

module RGSS

  RESOURCE_DIR = File.join(__dir__, 'resources')

  module Game

    def self.update(delta)
      $e.y -= 10 if Input.press?(:UP)
      $e.y += 10 if Input.press?(:DOWN)
      $e.x -= 10 if Input.press?(:LEFT)
      $e.x += 10 if Input.press?(:RIGHT)
      $e.z += 1 if Input.press?(:RAISE)
      $e.z -= 1 if Input.press?(:LOWER)

      $v.update(delta)
      $e.update(delta)
      $e.position = Input.cursor

      if Input.trigger?(:CLICK)
        p Input.hit_test(32, 32, 32, 32)
      end

      if Input.trigger?(:CONFIRM)
        $e.paused? ? $e.resume : $e.pause
        puts "PAUSED: #{$e.paused?}"
      end

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
  

  Game.icon = File.join(RESOURCE_DIR, 'ruby.png')
  Input.cursor_image(File.join(RESOURCE_DIR, 'cursor.png'))


  Input.bind(:CLICK, [], Input::MOUSE_BUTTON_LEFT)

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

  $v = Viewport.new(200, 200, 200, 200)

  $e = Emitter.new(20000, nil) do |e|
    e.velocity = vec3(0, 0, -0.00001)
    # e.opacity = 0.5
    # e.texture = Texture.load("/home/eric/Desktop/smoke.png")
    e.size = (2..6)
    e.position = vec2(1024 / 2, 768 / 2)
    e.radius = 2
    e.rate = 5
    e.direction = nil
    e.force = 100
    e.gravity = -80
    # e.wind = -50
    e.friction = 1

    e.growth = 2.1
    # e.rotation = 360
    e.fade = 10
    e.lifespan = 30 * 8
    # e.spectrum = (Color::GRAY..Color::WHITE)
    # e.scale = vec3(0.3, 0.3, 0)
  end





  Game.speed = 1.0
  begin
    Game.main(30)
  rescue
    Log.fatal($!.to_s)
    raise $1
  end
  Game.terminate
end
