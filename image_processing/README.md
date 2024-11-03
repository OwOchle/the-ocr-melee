# Image Processing

Le script de traitement d'image s'execute de cette manière:

`
./image_processing <fichier>
`

#### Il effectue les traitements suivants: 
- Convertion en nuances de gris
- Application du flou gaussien
- Application d'un seuil pour convertir l'image en  deux couleurs
- Application de l'opérateur de sobel
- Visualisation des lignes en X et Y

#### Les fichiers suivants sont écris dans `outputs`:
- output_blur.png
- output_threshold.png
- output_sobel.png
- output_lines.png