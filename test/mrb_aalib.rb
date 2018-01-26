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

