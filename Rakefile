require 'hoe'

require 'lib/nis/version.rb'

begin
  require 'jeweler'
  Jeweler::Tasks.new do |gem|
    gem.name = "nis"
    gem.summary = %Q{Provides an interface for NIS(YP) API.}
    gem.description = %Q{Provides an interface for NIS(YP) API.}
    gem.email = 'ttate@users.sourceforge.net'
    gem.homepage = 'http://raa.ruby-lang.org/project/ruby-nis/'
    gem.authors = ['Takaaki Tateishi']
    gem.version = NIS::VERSION
    gem.files.include "lib/nis/nis.so"
  end
  Jeweler::GemcutterTasks.new
rescue LoadError
  puts "Jeweler (or a dependency) not available. Install it with: gem install jeweler"
end

# HOE = Hoe.spec 'nokogiri' do
#   developer 'Takaaki Tateishi', 'ttate@users.sourceforge.net'
#   self.clean_globs = [
#     'ext/nis/*.{o,so,bundle,a,log,dll}',
#     'lib/nis/*.{o,so,bundle,a,log,dll}',
#   ]
#   self.spec_extras = { :extensions => ["ext/nis/extconf.rb"] }
# end
# Hoe.add_include_dirs '.'

task 'ext/nis/Makefile' do
  Dir.chdir('ext/nis') do
    ruby "extconf.rb"
  end
end

task 'lib/nis/nis.so' => 'ext/nis/Makefile' do
  Dir.chdir('ext/nis') do
    sh 'make'
  end
  FileUtils.cp 'ext/nis/nis.so', 'lib/nis'
end

task :build => 'lib/nis/nis.so'

task :default => [:build]
