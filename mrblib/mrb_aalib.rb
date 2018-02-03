module AAlib
  def self.setpalette(palette, index, red, green, blue)
    palette[index] = (red*30 + green*59 + blue*11)>>8
  end
end
