# mruby-aalib   [![Build Status](https://travis-ci.org/masahino/mruby-aalib.svg?branch=master)](https://travis-ci.org/masahino/mruby-aalib)
AAlib class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'masahino/mruby-aalib'
end
```
## example
```ruby
p AAlib.hi
#=> "hi!!"
t = AAlib.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
