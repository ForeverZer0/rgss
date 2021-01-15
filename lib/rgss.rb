
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

require_relative 'rgss/game_object'
require_relative 'rgss/shader'
require_relative 'rgss/texture'

require_relative 'rgss/blend'
require_relative 'rgss/batch'
require_relative 'rgss/renderable'
require_relative 'rgss/sprite'
require_relative 'rgss/viewport'
require_relative 'rgss/plane'


module RGSS


  module Game

    SPEED = 8

    def self.update(delta)

      $sprite.cy += 1 if Game.ticks % 8 == 0

      $v.y -= SPEED if Input.press?(:UP)
      $v.y += SPEED if Input.press?(:DOWN)
      $v.x -= SPEED if Input.press?(:LEFT)
      $v.x += SPEED if Input.press?(:RIGHT)
      $fog.zoom += vec2(0.05) if Input.press?(:RAISE)
      $fog.zoom -= vec2(0.05) if Input.press?(:LOWER)

      $sprite.update(delta)
      $v.update(delta)
      $fog.update(delta)
    
      if Input.trigger?(:FULLSCREEN)
        p Graphics.fps
        p $v.position
      end

      if Input.press?(:QUIT)
        Game.close(true)
      end
    end
  end

  module Graphics

    def self.render(state)
      @batch.each { |obj| obj.render(state) }
    end
  end


  Game.create(256, 256, "RGSS", resizable: true, vsync: true)
  Graphics.back_color = Color::CORNFLOWER_BLUE


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
  $v = Viewport.new(1, 1, 128, 128)
  $v.back_color = Color::BLACK

  $v.angle = 23
  
  $fog = Plane.new
  $fog.texture = Texture.new('/storage/images/RTP/XP/Graphics/Fogs/001-Fog01.png')
  $fog.size = Size.new(400, 400)
  $fog.scroll = vec2(24.0, 32.0)
  $fog.opacity = 0.5

  tex = Texture.new('/home/eric/Desktop/TimeFantasy_PCK/Characters/!$fireplace.png')
  
  w = tex.width / 3
  h = tex.height
  region = Rect.new(w, 0, w, h)


  $sprite = SpriteAtlas.new(region: region, rows: 4, columns: 1)
  $sprite.texture = tex

  font = TrueType.new('/home/eric/Desktop/dev/NotoSansMono-Regular.ttf')
  p font.scale(12, mode: :em)

  # tex.fill_rect(Rect.new(32, 32, 32, 32), Color::RED)
  # $sprite.scale = vec2(1, 1)
  # $sprite.velocity = vec2(64, 0)
  # $sprite.angle = 45
  # $sprite.pivot = vec3(16, 16, 0)
  # $sprite.scale = vec2(2, 2)


  # $v.angle = 45

  Game.speed = 1.0

  


  Game.main(30)
  Game.terminate

end
