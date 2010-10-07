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
