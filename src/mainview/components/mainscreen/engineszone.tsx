import { useState } from "react";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "../ui/tabs";
import { EngineAnalysis } from "./engineanalysis";

export function EnginesZone() {
    const [enginesIds, setEnginesIds] = useState<string[]>([])
    const [enginesAnalysis, setEnginesAnalysis] = useState<Record<string, string>>({})
    const [enginesLabels, setEnginesLabels] = useState<string[]>([])
    return (
        <div className="w-3/4 border-5 border-brown-600 rounded-sm">
            <Tabs>
                <TabsList>
                    <TabsTrigger value={enginesIds[0]}>{enginesLabels[0]}</TabsTrigger>
                    <TabsTrigger value={enginesIds[1]}>{enginesLabels[1]}</TabsTrigger>
                </TabsList>
                <TabsContent value={enginesIds[0]}><EngineAnalysis analysis={enginesAnalysis[enginesIds[0]]} /></TabsContent>
                <TabsContent value={enginesIds[1]}><EngineAnalysis analysis={enginesAnalysis[enginesIds[1]]} /></TabsContent>
            </Tabs>
        </div>
    );
}