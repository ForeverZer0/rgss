
module RGSS

  class Transition < Shader

    BASE_PATH = File.join(__dir__, 'shaders', 'transitions')

    def self.load(path, **opts)

      raise(ArgumentError, "path cannot be nil") unless path
        
      unless File.exist?(path)
        temp = File.join(BASE_PATH, path)
        raise(Errno::ENOENT, path) unless File.exist?(temp_path)
        path = temp
      end

      frag_src = File.read(path)
      new(frag_src, **opts)
    end

    def initialize(frag_source, **opts)

      vert_src = File.read(File.join(BASE_PATH, 'base-vertex.glsl'))
      frag_src = sprintf(File.read(File.join(BASE_PATH, 'base-fragment.glsl')), frag_source)

      super(vert_src, frag_source)

      uniforms.each_pair do |name, type|
        location = locate(name.to_s)
 
        define_method(name) do |value|
          uniform(location, value)
        end
      end

      private_constant :BASE_PATH
    end
  end
end