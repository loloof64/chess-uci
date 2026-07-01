import { electrobun } from "@/lib/electrobun";
import { useEffect, useState } from "react";
import { EnginesZone } from "./mainscreen/engineszone";
import { MainToolbar } from "./mainscreen/maintoolbar";

interface engineAnswer {
  id: number;
  line: string;
}

export function App() {
  const [enginesIds, setEnginesIds] = useState<string[]>([]);
  const [enginesAnalysis, setEnginesAnalysis] = useState<
    Record<string, string>
  >({});

  const addEngine = (id: number) =>
    setEnginesIds((oldEnginesIds) => [...oldEnginesIds, id.toString()]);

  useEffect(() => {
    const handler = (data: engineAnswer) => {
      setEnginesAnalysis((oldAnalysisRecord) => {
        const targetEngineId = data.id.toString();
        const currentTargetEngineAnalysis =
          oldAnalysisRecord[targetEngineId] ?? "";
        const newTargetEngineAnalysis = `${currentTargetEngineAnalysis}${data.line}`;
        return {
          ...oldAnalysisRecord,
          [targetEngineId]: newTargetEngineAnalysis,
        };
      });
    };

    electrobun.rpc?.addMessageListener("engineAnswer", handler);

    return () => {
      electrobun.rpc?.removeMessageListener("engineAnswer", handler);
    };
  }, [enginesIds]);

  return (
    <div className="min-h-screen flex flex-col items-center justify-start gap-6 p-8 bg-linear-to-tr from-orange-600 to-blue-800">
      <MainToolbar addEngineCb={addEngine} />
      <EnginesZone enginesIds={enginesIds} enginesAnalysis={enginesAnalysis} />
    </div>
  );
}
