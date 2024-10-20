import asyncio
import websockets
import keyboard

pressed_keys = {}

key_mapping = {
    "1": "left",
    "5": "right",
    "4": "up",
    "6": "down",
    "0": "enter",
    "2": "a",
    "3": "x",
}

async def key_hold_loop():
    while True:
        for key in pressed_keys.keys():
            keyboard.press(key)
        await asyncio.sleep(0.05)

async def handle_connection(websocket, path):
    print("Client connected")

    try:
        async for message in websocket:
            print(f"Received: {message}")
            if ":" not in message:
                continue
            button_index, action = message.split(":")
            
            if button_index in key_mapping:
                key = key_mapping[button_index]
  
                if action == "DOWN" and key not in pressed_keys:
                    # if we're pressing down a key that's already pressed, release it first
                    if key in pressed_keys:
                        keyboard.send(key_mapping[button_index], False, True)
                        del pressed_keys[key]
                        print(f"Released {key}")
                    # same for if we press "down" we should make sure "up" is released and vice versa. Same for left and right
                    if key_mapping[button_index] == "left" and "right" in pressed_keys:
                        keyboard.send("right", False, True)
                        del pressed_keys["right"]
                        print(f"Released right")
                    if key_mapping[button_index] == "right" and "left" in pressed_keys:
                        keyboard.send("left", False, True)
                        del pressed_keys["left"]
                        print(f"Released left")
                    if key_mapping[button_index] == "up" and "down" in pressed_keys:
                        keyboard.send("down", False, True)
                        del pressed_keys["down"]
                        print(f"Released down")
                    if key_mapping[button_index] == "down" and "up" in pressed_keys:
                        keyboard.send("up", False, True)
                        del pressed_keys["up"]
                        print(f"Released up")

                    keyboard.send(key_mapping[button_index], True, False)
                    pressed_keys[key] = True
                    print(f"Pressed {key}")
                elif action == "UP" and key in pressed_keys:
                    keyboard.send(key_mapping[button_index], False, True)
                    del pressed_keys[key]
                    print(f"Released {key}")
                
                print(f"Pressed keys: {pressed_keys.keys()}")

    except websockets.ConnectionClosed:
        print("Client disconnected")

async def main():
    server = websockets.serve(handle_connection, "0.0.0.0", 8080)
    await asyncio.gather(server, key_hold_loop())


print("WebSocket server started on port 8080")
# asyncio.get_event_loop().run_until_complete(start_server)
# asyncio.get_event_loop().run_forever()
asyncio.run(main())
