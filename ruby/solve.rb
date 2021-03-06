#!/usr/bin/env ruby

class Solver

  class Letter
    attr_accessor :letter, :row, :column

    def initialize letter, row, column
      @letter = letter
      @row = row
      @column = column
    end

    def to_s
      letter
    end
  end

  class View

    @@direction         = {-1 => {}, 0 => {}, 1 => {}}
    @@direction[1][0]   = 'North'
    @@direction[1][-1]  = 'Northeast'
    @@direction[0][-1]  = 'East'
    @@direction[-1][-1] = 'Southeast'
    @@direction[-1][0]  = 'South'
    @@direction[-1][1]  = 'Southwest'
    @@direction[0][1]   = 'West'
    @@direction[1][1]   = 'Northwest'


    def self.direction
      @@direction
    end

    attr_accessor :c, :matrix, :found

    def initialize matrix
      @matrix = matrix
      @found = {}
      setup
    end

    def direction letters
      self.class.direction[letters[0].row <=> letters[1].row][letters[0].column <=> letters[1].column]
    end

    def position letters
      "[#{letters[0].row},#{letters[0].column}]"
    end

    def include? word
      self.as_string.values.find {|s| s.include? word} && true || false
    end

    def as_string
      @as_string
    end

    def as_string=(s)
      @as_string = {forward: s, backward: s.reverse }
    end

    def as_array
      @as_array
    end

    def as_array=(a)
      @as_array = {forward: a, backward: a.reverse }
    end

    def found! word, where
      @found[word] = where
    end

    def found? word
      @found[word]
    end

    def locate word
      if (where = found? word)
        return where
      end

      as_string.each do |k,v|
        next if (i = v.index word).nil?
        letters = as_array[k].slice(i,word.length)
        where = "#{word} at #{position letters} #{direction letters}"
        return found! word, where
      end
      nil
    end
  end

  class RowView < View
    def setup
      self.as_string = matrix.join
      self.as_array = matrix.flatten
    end
  end

  class ColumnView < View
    def setup
      self.as_string = matrix.transpose.join
      self.as_array = matrix.transpose.flatten
    end
  end

  #
  # the diagonals
  #

  class DiagonalView < View
    def setup
      l=matrix.length-1
      h1=self.h1 l
      h2=self.h2 l
      h2.pop
      h2.reverse!
      w=h1+h2
      self.as_string=w.map {|e| e.map {|x,y| matrix[x][y]}.join}.join
      self.as_array=w.map {|e| e.map {|x,y| matrix[x][y]}}.flatten
      self.c=w
    end
  end

  #
  # slices starting at top left, ending bottom right
  #

  class LRDiagonalView < DiagonalView
    def h1 l; 0.upto(l).to_a.map {|i| i.downto(0).to_a.zip(0.upto(i)).to_a}; end
    def h2 l;l.downto(0).to_a.map {|i| l.downto(i).to_a.zip(l.downto(i).to_a.reverse)}; end
  end

  #
  # slices starting at top right, ending bottom left
  #

  class RLDiagonalView < DiagonalView
      def h1 l; 0.upto(l).to_a.map{|i| 0.upto(i).to_a.zip((l-i).upto(l)).to_a}; end
      def h2 l;l.downto(0).to_a.map{|i| i.upto(l).to_a.zip(0.upto(l-i)).to_a}; end
  end


  attr_accessor :view, :matrix

  def initialize
    self.matrix = IO.read("puzzle.matrix").split("\n").map.with_index{|line,i| line.split(//).map.with_index{|letter,j| Letter.new letter,i,j}}
    self.view = [RowView.new(matrix), ColumnView.new(matrix), LRDiagonalView.new(matrix), RLDiagonalView.new(matrix)]
  end

  def locate word
    view.each do |v|
      next if (where = v.locate word).nil?
      return where
    end
    nil
  end

  def solve
    f = File.open("/usr/share/dict/words", "r")
    f.each_line do |line|
      word = line.chomp.upcase
      next if (word.length < 5) or (where = locate word).nil?
      puts where
    end
    f.close
  end

end

def main
  s = Solver.new
  s.solve
end

if __FILE__ == $0
    main
end
