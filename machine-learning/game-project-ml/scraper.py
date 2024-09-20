from google_images_search import GoogleImagesSearch
from PIL import Image
import os

api_key = ""
search_engine_id = ""
gis = GoogleImagesSearch("", "")

games = {
    "minecraft": "minecraft gameplay screenshot", 
    "terraria": "terraria gameplay screenshot", 
    "lol": "league of legends gameplay screenshot", 
    "overwatch": "overwatch gameplay screenshot", 
    "fortnite": "fortnite gameplay screenshot", 
    "borderlands": "borderlands gameplay screenshot", 
    "gta": "grand theft auto gameplay screenshot", 
    "roblox": "roblox gameplay screenshot", 
    "wow": "world of warcraft gameplay screenshot", 
    "warframe": "warframe gameplay screenshot", 
}

for game, prompt in games.items():
    gis.search(search_params={'q': prompt, 'num': 50})

    for image in gis.results():
        image.download(f"./images/{game}")

    # Process Images
    game_dir = os.path.join(os.getcwd(), f"./images/{game}")
    iter = 0
    for filename in os.listdir(game_dir):
        img_path = os.path.join(game_dir, filename)
        out_path = os.path.join(game_dir, f"{game}_{iter}.png")

        img = Image.open(img_path)
        img_resized = img.resize((240, 240))
        img_resized.save(out_path)
        os.remove(img_path)

        iter += 1

    print(f"Finished downloading and processing images for: {game}")

