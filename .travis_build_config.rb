MRuby::Build.new do |conf|
  toolchain :gcc
  conf.gembox 'default'
  conf.gem File.expand_path(File.dirname(__FILE__))+"/mruby/mrbgems/mruby-io"
  conf.gem File.expand_path(File.dirname(__FILE__)) do |g|
    g.download_aalib
  end
  conf.enable_test
end
