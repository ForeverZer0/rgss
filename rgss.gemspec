require_relative 'lib/rgss/version'

Gem::Specification.new do |spec|
  spec.name          = 'rgss'
  spec.version       = RGSS::VERSION
  spec.authors       = ['Eric Freed']
  spec.email         = ['efreed09@gmail.com']

  spec.summary       = %q{High-performance Ruby game framework strongly influenced by the RPG Maker series by Enterbrain.}
  spec.description   = %q{
      IN ACTIVE DEVELOPMENT.
      High-performance Ruby game framework strongly influenced by the RPG Maker series by Enterbrain. The library contains
      multiple levels of abstraction, leveraging the power of modern OpenGL (C bindings built-in) and shaders, but building
      upon these low-level abstractions to create the user-friendly components that are to be expected in a 2D game framework, 
      including sprites, textures, fonts, colors, etc. Those experienced with low-level graphics APIs have all the tools at their
      fingertips already built-in (OpenGL, GLFW, OpenAL, sound/font/image decoding, etc) and ready to build their own engine from, 
      while those who have no interest in such can get started right away with powerful higher-level abstractions. While the 
      library's API surface similar in many ways to RPG Maker XP/VX/VXA, bear in mind that it is merely incluenced by those,
      and liberal design changes have been taken to not limit the API or add restriction.
    }
  spec.homepage      = 'https://github.com/ForeverZer0/rgss'
  spec.license       = 'MIT'
  spec.required_ruby_version = Gem::Requirement.new('>= 2.3.0')

  spec.metadata['homepage_uri'] = spec.homepage
  spec.metadata['source_code_uri'] = 'https://github.com/ForeverZer0/rgss'
  spec.metadata['changelog_uri'] = 'https://github.com/ForeverZer0/rgss/CHANGELOG.md'

  spec.files         = Dir.chdir(File.expand_path('..', __FILE__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  end

  spec.metadata['msys2_mingw_dependencies'] = 'glfw openal sndfile'

  spec.bindir        = 'bin'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']

  spec.add_development_dependency 'rake', '~> 13.0'
  spec.add_development_dependency 'rake-compiler', '~> 1.1'
  spec.add_development_dependency 'yard', '~> 0.9'

  spec.add_runtime_dependency 'ox', '~> 2.14'
end
