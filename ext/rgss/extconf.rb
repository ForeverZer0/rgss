require 'mkmf'
require 'fiddle'

CGLM_SHA256  = 'c96dd8a6f1d9aedb7ac02fe6bafd2a3a625a19e62bc18d455faf9c6825e9bd7b'
CGLM_VERSION = '0.7.9'
CGLM_URL     = "https://github.com/recp/cglm/archive/v#{CGLM_VERSION}.tar.gz"
CGLM_TAR_GZ  = File.expand_path("cglm-v#{CGLM_VERSION}.tar.gz", __dir__)
CGLM_DIR     = File.expand_path("cglm-#{CGLM_VERSION }", __dir__)

# fetch cglm unless it's already on the file system
unless Dir.exist?(CGLM_DIR)

  require 'open-uri'
  require 'digest'
  require 'fileutils'
  require 'rubygems/package'
  require 'zlib'

  # Fetch tarball
  open(CGLM_TAR_GZ, 'wb') do |file|
    file << URI.open(CGLM_URL, 'rb').read
  end

  # Check file integrity
  sha256 = Digest::SHA256.file(CGLM_TAR_GZ)
  abort("invalid checksum for '#{CGLM_TAR_GZ}'") unless sha256 == CGLM_SHA256

  # Extract TAR archive
  FileUtils.rm_rf(CGLM_DIR) if File.directory?(CGLM_DIR)
  FileUtils.mkdir_p(CGLM_DIR)
  Gem::Package::TarReader.new(Zlib::GzipReader.open(CGLM_TAR_GZ)) do |tar|
    tar.rewind
    tar.each do |entry|
      path = File.expand_path(entry.full_name, __dir__)
      if entry.directory?
        FileUtils.mkdir_p(path)
      else
        FileUtils.mkdir_p(File.dirname(path))
        File.open(path, 'wb') { |file| file << entry.read }
      end
    end
  end
  FileUtils.remove_file(CGLM_TAR_GZ, true) rescue nil
end

$INCFLAGS << " -I#{CGLM_DIR}/include"

%i(
  SIZEOF_VOIDP SIZEOF_CHAR SIZEOF_SHORT SIZEOF_INT SIZEOF_LONG SIZEOF_LONG_LONG
  SIZEOF_FLOAT SIZEOF_DOUBLE SIZEOF_SIZE_T SIZEOF_SSIZE_T SIZEOF_PTRDIFF_T
  SIZEOF_INTPTR_T SIZEOF_UINTPTR_T
).each do |const|
  next unless Fiddle.constants.include?(const)
  $CFLAGS << " -DHAVE_#{const}=1 -D#{const}=#{Fiddle.const_get(const)}"
end

pkg_config('glfw3')
pkg_config('pangoft2')
pkg_config('sndfile')
pkg_config('openal')

create_makefile("rgss/rgss")
