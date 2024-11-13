import cv2
import os
from natsort import natsorted  # Pour trier les fichiers par nom de maniere naturelle

# Chemin du dossier contenant les images
images_folder = "chemin/vers/le/dossier_images"
output_video = "timelapse_video.mp4"

# Definissez la frequence d'images (nombre d'images par seconde)
fps = 30

# Obtenez tous les fichiers dans le dossier et filtrez les images (ex : .jpg, .png)
images = [img for img in os.listdir(images_folder) if img.endswith(".jpg") or img.endswith(".png")]
images = natsorted(images)  # Tri naturel des images par nom

# Verification qu'il y a des images dans le dossier
if not images:
    print("Aucune image trouvee dans le dossier specifie.")
    exit()

# Recuperation des dimensions d'une image pour la taille de la video
first_image_path = os.path.join(images_folder, images[0])
frame = cv2.imread(first_image_path)
height, width, layers = frame.shape
frame_size = (width, height)

# Initialisation du writer video
fourcc = cv2.VideoWriter_fourcc(*"mp4v")  # Codec pour la video .mp4
video_writer = cv2.VideoWriter(output_video, fourcc, fps, frame_size)

# Ajout de chaque image à la video
for image in images:
    image_path = os.path.join(images_folder, image)
    frame = cv2.imread(image_path)
    if frame is None:
        print(f"Image non trouvee ou inaccessible : {image_path}")
        continue
    video_writer.write(frame)  # ecriture de la frame dans la video

# Liberation des ressources
video_writer.release()
print(f"Video timelapse creee avec succes : {output_video}")