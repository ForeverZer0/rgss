
module RGSS

  require 'logger'

  Log = Logger.new(STDOUT)

  Log.formatter = proc do |severity, datetime, progname, msg|

    time = datetime.strftime('%H:%M:%S')
    prefix = case severity
    when 'DEBUG' then "\33[1;35mDEBUG\33[0m"
    when 'INFO' then "\33[1;32mINFO\33[0m"
    when 'WARN' then "\33[1;33mWARN\33[0m"
    when 'ERROR' then "\33[1;31mERROR\33[0m"
    when 'FATAL' then "\33[1;31mFATAL\33[0m"
    else "LOG"
    end
    "#{time} [#{prefix}] #{msg}\n"
  end
end

require_relative 'rgss/version'
require_relative 'rgss/rgss'

# require_relative 'rgss/renderable'
require_relative 'rgss/sprite'
require_relative 'rgss/viewport'
require_relative 'rgss/plane'
require_relative 'rgss/font'


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

      $sprite.update(delta)
      # $v.update(delta)
      # $fog.update(delta)
    
      if Input.trigger?(:FULLSCREEN)
        p Graphics.fps
        p $v.position
      end

      if Input.press?(:QUIT)
        Game.close(true)
      end
    end
  end



  Game.create(1024, 768, "RGSS", resizable: true, vsync: true)
  Graphics.back_color = Color::DARK_SLATE_GRAY


  Game.icon = '/storage/images/anvil.png'

  Input.bind(:UP, [Input::KEY_W, Input::KEY_UP])
  Input.bind(:LEFT, [Input::KEY_A, Input::KEY_LEFT])
  Input.bind(:DOWN, [Input::KEY_S, Input::KEY_DOWN])
  Input.bind(:RIGHT, [Input::KEY_D, Input::KEY_RIGHT])
  Input.bind(:RAISE, Input::KEY_Q)
  Input.bind(:LOWER, Input::KEY_E)
  Input.bind(:CONFIRM, Input::KEY_SPACE)
  Input.bind(:FULLSCREEN, Input::KEY_F)
  Input.bind(:QUIT, Input::KEY_ESCAPE)

  Input.cursor_image('/home/eric/Desktop/cursor.png')
  # $v = Viewport.new(1, 1, 256, 256)
  # $v.back_color = Color::BLACK
  # $v.position = vec2(400, 100)

  # # GL.glPixelStorei(GL::GL_UNPACK_ALIGNMENT, 4)
  # fog_tex = Texture.load('/storage/images/RTP/XP/Graphics/Fogs/001-Fog01.png')

  # # $v.angle = 23
  # $fog = Plane.new
  # $fog.texture = fog_tex
  # $fog.size = Size.new(1024, 768)
  # $fog.scroll = vec2(24.0, 32.0)
  # $fog.opacity = 0.5
 
  # tex = Texture.load('/home/eric/Desktop/TimeFantasy_PCK/Characters/!$fireplace.png')
  # $sprite = Sprite.new(viewport: $v)
  # $sprite.texture = tex

  
  markup = <<-EOS
regular <b>bold</b> <i>italic</i> <u>underline</u> <span overline="single">overline</span>
<s>strikethrough</s> <tt>monospace</tt> <b><u>bold underline</u></b> 
<span font_style="oblique">oblique</span> <small>smaller</small> <big>bigger</big> <sup>superscript</sup> <sub>subscript</sub>
<span font="Comic Sans MS">Mid-string font change</span>
<span foreground=\"#FFCC00\"> colored</span>
EOS

  Font.add_file('/home/eric/open_rpg/fonts/MinecraftCHMC.ttf')

  font = Font.new('MinecraftCHMC', 32)
  size = font.measure(markup)
  baked = font.bake(markup, size, align: Font::ALIGN_CENTER)
  # tex = Texture.new(size.width, size.height, baked, format: GL::GL_RED, internal: GL::GL_RED)

  # $sprite = TextSprite.new()
  # $sprite.texture = tex
  # $sprite.color = Color::WHITE

  args = Font.test_texture(markup, 'MinecraftCHMC 18', Size.empty)
  $sprite = Sprite.new()
  $sprite.texture = Texture.wrap(*args)
  # $sprite.color = Color::WHITE





  Game.speed = 1.0
  Game.main(30)
  Game.terminate

end
