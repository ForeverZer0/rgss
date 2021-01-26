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

      $fog.update(delta)
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
        $e.fullscreen
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


  temp = Texture.load('/home/eric/Desktop/test.png')
  fog_tex = Texture.load("/storage/images/RTP/XP/Graphics/Fogs/001-Fog01.png")

  fog_tex.blit(0, 0, temp, 0.2)

  $fog = Plane.new
  $fog.texture = fog_tex
  $fog.size = Size.new(1024, 768)
  $fog.scroll = vec2(32.0, 12.0)
  $fog.opacity = 0.80
  $fog.visible = true

  markup = <<-EOS
regular <b>bold</b> <i>italic</i> <u>underline</u> <span overline="single">overline</span>
<s>strikethrough</s> <tt>monospace</tt> <b><u>bold underline</u></b> 
<span font_style="oblique">oblique</span> <small>smaller</small> <big>bigger</big> <sup>superscript</sup> <sub>subscript</sub>
<span font="Comic Sans MS">Mid-string font change</span>
<span foreground=\"#FFCC00\"> colored</span>
EOS

  $v = Viewport.new(200, 200, 200, 200)
  font = Font.new('Noto Mono 18')
  puts font

  $e = Emitter.new(20000, nil) do |e|
    e.opacity = 0.75
    # e.texture = Texture.load("/home/eric/Desktop/smoke.png")
    e.particle_size = (4..6)
    e.position = vec2(1024 / 2, 768 / 2)
    e.radius = 4
    e.rate = 50
    e.direction = 170..190
    

    # e.friction = 12
    e.force = 100
    e.gravity = 50
    # e.wind = rand(10, 30)
    e.order = Emitter::ORDER_OLDEST
    # e.growth = (6..20)
    e.rotation = -40..40
    e.fade = 25
    e.lifespan = 30 * 8
    e.spectrum = (Color::BLUE..Color::ALICE_BLUE)
    e.frequency = 5
  end






  Game.speed = 1.0
  begin
    Game.main(30)
  rescue
    Log.fatal($!.to_s)
    raise $!
  end
  Game.terminate
end
