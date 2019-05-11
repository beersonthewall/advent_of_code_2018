class day2():
    """Day 2 solutions for advent of code 2018"""
    def __init__(self, input):
        self.ids = [line for line in open(input)]
        self.counts = [self.process_id(line) for line in open(input)]

    def process_id(self, id):
        """Return a dictionary with the letters in an id as the keys
        and the number of occurrences for that letter in the id as the value"""
        letter_cnt = {}
        for letter in id:
            if letter in letter_cnt:
                letter_cnt[letter] += 1
            else:
                letter_cnt[letter] = 1
        return letter_cnt

    def solve_part1(self):
        occur_twice = 0
        occur_thrice = 0
        for count in self.counts:
            if 2 in count.values():
                occur_twice += 1
            if 3 in count.values():
                occur_thrice += 1
        print(occur_twice * occur_thrice)

    def solve_part2(self):
        self.ids.sort()
        index = 0
        max_l = len(self.ids)
        while index < max_l-2:
            fst = self.ids[index]
            snd = self.ids[index+1]
            if self.dist(fst, snd) == 1:
                shared = ""
                for i in range(len(fst)):
                    if fst[i] == snd[i]:
                        shared = shared + fst[i]
                print(shared)
            index += 1

    def dist(self, fst, snd):
        l = len(fst)
        diff = 0
        for i in range(l):
            if fst[i] != snd[i]:
                diff += 1
        return diff

two = day2("input/day2_input.txt")
two.solve_part1()
two.solve_part2()
