class day1():
    """Day 1 solutions for advent of code 2018"""
    def __init__(self, input):
        nums = list(map(lambda x: int(x, 10), open(input)))
        self.nums = nums

    def solve_part1(self):
        print(sum(self.nums))

    def solve_part2(self):
        sum = 0
        seen = {sum}
        while True:
            for n in self.nums:
                sum += n
                if sum in seen:
                    print(sum)
                    return
                seen.add(sum)


one = day1("input/day1_input.txt")
one.solve_part1()
one.solve_part2()
