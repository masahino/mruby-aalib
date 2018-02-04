MRuby::Gem::Specification.new('mruby-aalib') do |spec|
  spec.license = 'MIT'
  spec.authors = 'masahino'

  def spec.download_aalib
    require 'open-uri'
    aalib_url = "https://downloads.sourceforge.net/aa-project/aalib-1.4rc5.tar.gz"
    aalib_patch = "https://raw.githubusercontent.com/Homebrew/formula-patches/6e23dfb/aalib/1.4rc5.patch"
    aalib_build_root = "#{build_dir}/aalib/"
    aalib_dir = "#{aalib_build_root}/aalib-1.4.0"
    aalib_a = "#{aalib_dir}/src/.libs/libaa.a"
    cppflags = ""

    unless File.exists?(aalib_a)
      open(aalib_url, "r") do |http|
        aalib_tar = http.read
        FileUtils.mkdir_p aalib_build_root
        IO.popen("tar xfz - -C #{filename aalib_build_root}", "w") do |f|
          f.write aalib_tar
        end
      end
      if RUBY_PLATFORM =~ /darwin/i
        sh %Q{cd #{aalib_dir} && wget #{aalib_patch} && patch -p1 < 1.4rc5.patch}
        cppflags="-DNCURSES_OPAQUE=0"
      end
      sh %Q{cd #{aalib_dir} && CPPFLAGS=#{cppflags} ./configure --without-x && make}
    end
    self.cc.include_paths << aalib_dir+"/src/"
    self.linker.flags_before_libraries << aalib_a
  end

end
