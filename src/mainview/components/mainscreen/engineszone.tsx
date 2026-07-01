import { Tabs, TabsContent, TabsList, TabsTrigger } from "../ui/tabs";
import { EngineAnalysis } from "./engineanalysis";

interface EnginesZoneProps {
  enginesIds: string[];
  enginesAnalysis: Record<string, string>;
}

export function EnginesZone({ enginesIds, enginesAnalysis }: EnginesZoneProps) {
  return (
    <div className="w-full border-5 border-brown-600 rounded-sm">
      <Tabs>
        <TabsList>
          {enginesIds.map((currentEngineId: string) => {
            return (
              <TabsTrigger value={currentEngineId}>
                {currentEngineId}
              </TabsTrigger>
            );
          })}
        </TabsList>
        {enginesIds.map((currentEngineId: string) => {
          return (
            <TabsContent value={currentEngineId}>
              <EngineAnalysis analysis={enginesAnalysis[currentEngineId]} />
            </TabsContent>
          );
        })}
      </Tabs>
    </div>
  );
}
