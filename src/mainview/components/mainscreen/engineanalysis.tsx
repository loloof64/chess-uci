interface EngineAnalysisProps {
    analysis: string
}

export function EngineAnalysis({analysis} : EngineAnalysisProps) {
    return (
        <textarea readOnly cols={300} rows={20}>{analysis}</textarea>
    )
}