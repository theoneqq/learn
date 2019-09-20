MAX_CARD_NUM = 7
from_type_cards = 1
from_type_table = 2

card_type_follower = 1
card_type_magic = 2

buff_effect_type_taunt = 1
buff_effect_type_add_attribute = 2

buff_target_type_self = 1
buff_target_type_all = 2

cards = {
    1 : { 'card_type' : 1, 'hp' : 10, 'hp_limit' : 10, 'attack' : 10, 'stone' : 10, 'has_target' : True},        
}

buffs = {
    1 : { 'buff_type' : 1 }        
}

class card:
    def __init__(self, card_id):
        card_data = cards[card_id]
        for key, value in card_data.items():
            setattr(self, key, value)
            self.buffs = {}

class buff:
    def __init__(self, buff_id):
        pass

class player:
    def __init__(self, side, stone):
        self.side = side
        self.stone = stone
        self.cards = []
        self.table = []

class match:
    def __init__(self):
        self.players = {}

    def add_player(self, player):
        side = player.side
        self.players[side] = player

    def use_card(self, player, from_type, pos, target_player, target_pos):
        card = players.table[pos] if from_type == from_type_table else player.cards[pos]
        if target_player != None and target_pos != None:
            target_card = target_player.table[target_pos]

p1 = player(1, 8)
p2 = player(2, 10)

new_match = match()
new_match.add_player(p1)
new_match.add_player(p2)
