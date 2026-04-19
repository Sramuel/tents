// variables pour le jeu et le canvas
const EMPTY = 0, TREE = 1, TENT = 2, GRASS = 3, BIGTREE = 4;
var gameInstance; 
var canvas = document.getElementById('mycanvas');

// chargement des images
var imgTree = new Image(); imgTree.src = 'res/tree_spring.png';
var imgTent = new Image(); imgTent.src = 'res/tent_spring.png';
var imgGrass = new Image(); imgGrass.src = 'res/grass_spring.png';
var imgBigTree = new Image(); imgBigTree.src = 'res/bigtree_spring.png';
var imgGrid = new Image(); imgGrid.src = 'res/grid_spring.png';


// enregistrement des évènements
Module.onRuntimeInitialized = () => {
    console.log("lancement du jeu...");
    start(); 
};

// gestion du clic sur le canvas
canvas.addEventListener('click', canvasLeftClick);

// callbacks des évènements
function start() {
    // création de l'instance par défaut
    gameInstance = Module._new_default();
    
    // pour voir la grille dès le chargement de la page, on s'assure que la dernière image à charger est prête
    if (imgGrid.complete) {
        refreshUI();
    } else {
        imgGrid.onload = () => { refreshUI(); };
    }
    // liaison des boutons html
    document.getElementById('btn-undo').onclick = () => { Module._undo(gameInstance); refreshUI(); };
    document.getElementById('btn-redo').onclick = () => { Module._redo(gameInstance); refreshUI(); };
    document.getElementById('btn-restart').onclick = () => { Module._restart(gameInstance); refreshUI(); };
    document.getElementById('btn-solve').onclick = () => { Module._solve(gameInstance); refreshUI(); };
    document.getElementById('btn-random').onclick = () => {
        // on supprime l'ancienne instance
        Module._delete(gameInstance);
        // paramètres aléatoires du nouveau jeu
        const w = Math.floor(Math.random() * (10 - 3 + 1)) + 3;
        const h = Math.floor(Math.random() * (10 - 3 + 1)) + 3;
        const wrapped = Math.random() > 0.5;
        const surface = w * h; // nombre total de cases
        // entre 10 et 25% de la grille
        const nb_arbres = Math.floor(Math.random() * (surface / 4 - 10 + 1)) + 10;
        // entre 0 et 10% de la grille
        const nb_big = Math.floor(Math.random() * (surface / 10 + 1));
        // on crée le nouveau jeu randomisé
        gameInstance = Module._new_random(w, h, wrapped, nb_arbres, nb_big, false);
        refreshUI();
    };
}

function refreshUI() {
    // mise à jour du dessin et du message d'état
    drawCanvas();
    var status = document.getElementById('status-message');
    if (Module._won(gameInstance)) {
        status.innerHTML = "<b>Congratulations !</b>";
    } else {
        status.innerHTML = "<b>Let's play !<b>";
    }
}

function canvasLeftClick(event) {
    const nb_lignes = Module._nb_rows(gameInstance);
    const nb_colonnes = Module._nb_cols(gameInstance);
    
    // on retire 40px pour laisser de la place aux chiffres (marge de sécurité)
    const zone_w = canvas.width - 40;
    const zone_h = canvas.height - 40;

    // calcul de la taille de case
    const taille_case = Math.min(zone_w / nb_colonnes, zone_h / nb_lignes);

    // calcul des décalages pour centrer la grille
    const decalage_x = (zone_w - (nb_colonnes * taille_case)) / 2;
    const decalage_y = (zone_h - (nb_lignes * taille_case)) / 2;

    // récupération des coordonnées i,j (en soustrayant le décalage)
    // offsetX/Y correspondent à la position de la souris dans le canvas
    const j = Math.floor((event.offsetX - decalage_x) / taille_case);
    const i = Math.floor((event.offsetY - decalage_y) / taille_case);

    if (i >= 0 && i < nb_lignes && j >= 0 && j < nb_colonnes) {
        let contenu_case = Module._get_square(gameInstance, i, j);
        if (contenu_case !== TREE && contenu_case !== BIGTREE) {
            let val_suivante = (contenu_case === EMPTY) ? TENT : (contenu_case === TENT ? GRASS : EMPTY);
            Module._play_move(gameInstance, i, j, val_suivante);
            refreshUI();
        }
    }
}

// dessins sur le canvas

function drawCanvas() {
    var ctx = canvas.getContext('2d');

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    const nb_lignes = Module._nb_rows(gameInstance);
    const nb_colonnes = Module._nb_cols(gameInstance);
    const zone_w = canvas.width - 40;
    const zone_h = canvas.height - 40;
    const taille_case = Math.min(zone_w / nb_colonnes, zone_h / nb_lignes);
    const decalage_x = (zone_w - (nb_colonnes * taille_case)) / 2;
    const decalage_y = (zone_h - (nb_lignes * taille_case)) / 2;

    for (var i = 0; i < nb_lignes; i++) {
        for (var j = 0; j < nb_colonnes; j++) {
            var contenu = Module._get_square(gameInstance, i, j);
            var x = decalage_x + j * taille_case;
            var y = decalage_y + i * taille_case;

            // grille de fond
            ctx.drawImage(imgGrid, x, y, taille_case, taille_case);

            // éléments à afficher
            if (contenu === TREE) ctx.drawImage(imgTree, x, y, taille_case, taille_case);
            else if (contenu === TENT) ctx.drawImage(imgTent, x, y, taille_case, taille_case);
            else if (contenu === GRASS) ctx.drawImage(imgGrass, x, y, taille_case, taille_case);
            else if (contenu === BIGTREE) ctx.drawImage(imgBigTree, x, y, taille_case, taille_case);
        }
    }
    drawNumbers(ctx, decalage_x, decalage_y, taille_case, nb_lignes, nb_colonnes);
}

function drawNumbers(ctx, decalage_x, decalage_y, taille_case, nb_lignes, nb_colonnes) {
    ctx.save();
    ctx.fillStyle = "red"; 
    ctx.font = `bold ${taille_case * 0.4}px Arial`;
    ctx.textAlign = "center";
    ctx.textBaseline = "middle";

    // chiffres des lignes (à droite de la grille)
    for (let i = 0; i < nb_lignes; i++) {
        const val = Module._get_expected_nb_tents_row(gameInstance, i);
        const x_pos = decalage_x + (nb_colonnes * taille_case) + 15;
        const y_pos = decalage_y + (i * taille_case) + (taille_case / 2);
        ctx.fillText(val, x_pos, y_pos);
    }
    // chiffres des colonnes (en bas de la grille)
    for (let j = 0; j < nb_colonnes; j++) {
        const val = Module._get_expected_nb_tents_col(gameInstance, j);
        const x_pos = decalage_x + (j * taille_case) + (taille_case / 2);
        const y_pos = decalage_y + (nb_lignes * taille_case) + 15;
        ctx.fillText(val, x_pos, y_pos);
    }
    ctx.restore();
}