class Animal
  constructor: (@name) ->

  move: (meters) ->
    res.write @name + " moved #{meters}m.\n"

class Snake extends Animal
  move: ->
    res.write "Slithering...\n"
    super 5

class Horse extends Animal
  move: ->
    res.write "Galloping...\n"
    super 45