#!/usr/bin/python3

def cmp(a, b):
    return (a > b) - (a < b) 

class AutoVivification(dict):
    """Implementation of perl's autovivification feature."""
    def __getitem__(self, item):
        try:
            return dict.__getitem__(self, item)
        except KeyError:
            value = self[item] = type(self)()
            return value

class Solver(object):
    class Letter(object):

        _direction = AutoVivification()
        _direction[1][0]   = 'North'
        _direction[1][-1]  = 'Northeast'
        _direction[0][-1]  = 'East'
        _direction[-1][-1] = 'Southeast'
        _direction[-1][0]  = 'South'
        _direction[-1][1]  = 'Southwest'
        _direction[0][1]   = 'West'
        _direction[1][1]   = 'Northwest'

        def __init__(self, letter=None, row=None, column=None):
            self.letter = letter
            self.row = row
            self.column = column

        def position(self):
            return "[%d,%d]" % (self.row, self.column)

        def direction(self, other):
            return self._direction[cmp(self.row,other.row)][cmp(self.column,other.column)]


    class View(object):
        def __init__(self, solver):
            self.solver = solver
            s = ''.join([l.letter for row in self.show() for l in row])
            l = [l for row in self.show() for l in row]
            self.as_string = {'forwards': s, 'backwards' : s[::-1]}
            self.as_list = {'forwards': l, 'backwards' : l[::-1]}

        def direction(self, word):
            return word[0].direction(word[1])

        def position(self,word):
            return word[0].position()

        def locate(self,word):
            for k,v in self.as_string.items():
                i = v.find(word)
                if (i == -1):
                    continue
                letters = self.as_list[k][i:i+len(word)]
                where = "%s at %s %s" % (word, self.position(letters), self.direction(letters))
                return where
            return None

    class RowView(View):
        def show(self):
            return self.solver.matrix

    class ColumnView(View):
        def show(self):
            return map(list, zip(*self.solver.matrix))

    class DiagonalView(View):
        def show(self):
            l=len(self.solver.matrix)
            h1=self.h1(l)
            h2=self.h2(l)
            h2.pop()
            h2.reverse()
            w=h1+h2
            return [[self.solver.matrix[x][y] for (x,y) in list(group)] for group in w]

    class LRDiagonalView(DiagonalView):
        def h1(self,l): return [zip(range(i,-1,-1),range(i+1)) for i in range(l)]
        def h2(self,l): return [zip(range(l-1,i-1,-1),range(i,l)) for i in range(l-1,-1,-1)]

    class RLDiagonalView(DiagonalView):
        def h1(self,l): return [zip(range(i+1),range(l-i-1,l)) for i in range(l)]
        def h2(self,l): return [zip(range(i,l),range(l-1,i-1,-1)) for i in range(l-1,-1,-1)]

    def __init__(self):
        with open("puzzle.matrix") as f:
          lines = (line.rstrip() for line in f)
          matrix = list(list(line) for line in lines if line)
          self.matrix = [[Solver.Letter(letter,i,j) for j,letter in enumerate(row)] for i,row in enumerate(matrix)]
          self.view = [ Solver.RowView(self), Solver.ColumnView(self), Solver.LRDiagonalView(self), Solver.RLDiagonalView(self) ]

    def locate(self,word):
        for v in self.view:
            where = v.locate(word)
            if where is None:
                continue
            return where
        return None

    def solve(self):
        with open("/usr/share/dict/words") as f:
            words = (word.rstrip().upper() for word in f)
            words = (word for word in words if len(word) > 4)
            for word in words:
                where = self.locate(word)
                if (where is None):
                    continue
                print(where)

def main():
    s=Solver()
    s.solve()

if __name__ == "__main__":
    main()
