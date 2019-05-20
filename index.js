const join = require('path').join;
const hidemu = require(join(__dirname, 'build/Release/hidemu.node'));

hidemu.mouseMoveSmoothly = function (x, y) {

    const delay = hidemu.mouseDelay();
    hidemu.mouseDelay(Math.ceil(Math.random() * 3) + 1);

    const cPos = hidemu.mousePos();

    const disX = x - cPos.x;
    const disY = y - cPos.y;

    const frame = Math.abs(disX > disY ? disY : disX);

    for (let i = 1; i < frame + 1; i++) {
        hidemu.mouseMove(cPos.x + Math.ceil(disX / frame * i), cPos.y + Math.ceil(disY / frame * i))
    }

    hidemu.mouseDelay(delay);

}

module.exports = hidemu;