global.main_action = () ->
    res.write '<pre>'
    sam = new Snake "Sammy the Python"
    tom = new Horse "Tommy the Palomino"

    sam.move()
    tom.move()
    res.stop()
