declare module "hidemu" {

    interface MousePosition {
        x: number;
        y: number;
    }

    function mouseDelay(delay: number): void;

    function mousePos(): MousePosition;

    function mouseMove(x: number, y: number): boolean;

    function mouseDown(isRight?: boolean): void;

    function mouseUp(isRight?: boolean): void;

    function mousePress(isRight?: boolean): void;

    function mouseWheel(delta: number, isHorizon?: boolean): void;

}