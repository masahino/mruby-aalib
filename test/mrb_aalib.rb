##
## AAlib Test
##

assert("putpixel") do
  aa = AAlib::Context.new("stdout")
  aa.putpixel(0,0,0)
  aa.close
end

assert("AAlib::Context#scrwidth") do
  aa = AAlib::Context.new("stdout")
  assert_equal(80, aa.scrwidth)
  aa.close
end

assert("AAlib::Context#initkbd") do
  aa = AAlib::Context.new("stdout")
#  assert_equal(1, aa.initkbd(0));
#  assert_raise(aa.initkbd(0, "hoge"))
end