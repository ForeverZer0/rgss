require 'rake/extensiontask'

task build: :compile

Rake::ExtensionTask.new('rgss') do |ext|
  ext.lib_dir = 'lib/rgss'
end

task default: %i[clobber compile]
