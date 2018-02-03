c = AAlib::Context.new()
c.initkbd(0)
for i in 0..(c.imgwidth-1)
  for y in 0..(c.imgheight-1)
    c.putpixel(i, y, (i + y < 80)? i: (((i + y) < 100)? ((i + y == 89)? 150 : 0) : (y * 8)))
  end
end
c.hidecursor
c.fastrender(0, 0, c.scrwidth, c.scrheight)
c.puts(0, 0, AAlib::SPECIAL, "Fast rendering routine 1")
c.flush()
c.getkey(1)
s = "line editor."
s = c.edit(0, 1, 20, s, 256)
c.puts(0, 0, AAlib::SPECIAL, "Key lookup test        ")
c.flush()

while (c.getkey(0) == AAlib::NONE) do
  true 
end
if (c.initmouse(AAlib::MOUSEALLMASK))
  co = 0
  s = "Mouse test-space to exit"
  c.puts(0, 0, AAlib::SPECIAL, s)
  c.flush()
  while (c.getevent(1) != ' '.ord)
    co += 1
    ret = c.getmouse()
    x = ret[0]
    y = ret[1]
    b = ret[2]
    s = "Mouse test-space to exit. x:#{x} y:#{y} b:#{b} event ##{co}  "
    c.puts(0, 0, AAlib::SPECIAL, s)
    c.flush()
  end
  c.hidemouse();

  while (c.getevent(1) != ' '.ord)
    co += 1
    ret = c.getmouse()
    x = ret[0]
    y = ret[1]
    b = ret[2]
    s = "Hidden mouse test-space to exit. x:#{x} y:#{y} b:#{b} event ##{co}  "
    c.puts(0, 0, AAlib::SPECIAL, s)
    c.flush()
  end
  c.uninitmouse()
end
c.close
