declare module "hidemu" {

    interface MousePosition {
        x: number;
        y: number;
    }

    /**
     * 设置全局鼠标操作后的延迟
     * @param delay 单位是微秒
     */
    function mouseDelay(delay?: number): number;

    /**
     * 获取鼠标位置
     */
    function mousePos(): MousePosition;

    /**
     * 鼠标瞬间移动到指定位置
     */
    function mouseMove(x: number, y: number): boolean;

    /**
     * 鼠标平滑移动到指定位置
     */
    function mouseMoveSmoothly(x: number, y: number): boolean;

    /**
     * 按下鼠标
     */
    function mouseDown(isRight?: boolean): void;

    /**
     * 抬起鼠标
     */
    function mouseUp(isRight?: boolean): void;

    /**
     * 按下（一定时间）后抬起鼠标
     */
    function mousePress(isRight?: boolean, duration?: number): void;

    /**
     * 鼠标竖直（横向）滚动
     */
    function mouseWheel(delta: number, isHorizon?: boolean): void;

}