import { Project, Node } from "ts-morph";

const project = new Project({
    useInMemoryFileSystem: true,
    compilerOptions: {
        noEmitOnError: false
    }
});

const file = project.createSourceFile(
    "test.d.ts",
    `
interface iTest {
  field: string;
  action: (a: string, b?: number) => void;
}
type access = iTest['action'];
`
);

file.forEachDescendant(node => {
    if (Node.isIndexedAccessTypeNode(node)) {
        console.log(node.getIndexTypeNode().getKindName());
        console.log(node.getType().getSymbol()?.getDeclarations()[0].getKindName())
    }
});
