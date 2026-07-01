import { useCallback } from "react";
import { electrobun } from "@/lib/electrobun";

interface MainToolbarButtonProps {
  text: string;
  clickCb: () => any;
}

function MainToolbarButton({ text, clickCb }: MainToolbarButtonProps) {
  return (
    <button
      onClick={clickCb}
      className="size-[60px] bg-gray-200 opacity-75 text-orange-900 flex justify-center items-center text-5xl"
    >
      {text}
    </button>
  );
}

interface MainToolbarProps {
  addEngineCb: (id: number) => void;
}

export function MainToolbar({ addEngineCb }: MainToolbarProps) {
  const addEngineFn = useCallback(async () => {
    const engineId = await electrobun.rpc?.request.createEngine();
    addEngineCb(engineId);
    await electrobun.rpc?.request.sendEngineCommand({
      id: engineId,
      command: "uci",
    });
    await electrobun.rpc?.request.sendEngineCommand({
      id: engineId,
      command: "position startpos",
    });
    await electrobun.rpc?.request.sendEngineCommand({
      id: engineId,
      command: "go movetime 10000",
    });
  }, [addEngineCb]);

  return (
    <div className="w-2/3 h-[80px] border-5 border-amber-200 rounded-full flex justify-center items-center">
      <MainToolbarButton text="+" clickCb={addEngineFn} />
    </div>
  );
}
