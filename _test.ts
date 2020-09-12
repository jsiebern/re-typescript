import { Project, Node, ts } from 'ts-morph';
// import * as c from '@ts-morph/common';

const project = new Project({
  useInMemoryFileSystem: true,
  compilerOptions: {
    noEmitOnError: false,
  },
});

const file = project.createSourceFile(
  'test.d.ts',
  `
  // type x<a> = a extends string ? string : number;
  // type y = x<string>
  // type z = x<boolean>

  type animation = 'overlay' | 'push' | 'scale down' | 'uncover' | 'slide out' | 'slide along'

export declare type Subset<T, U> = {
  [key in keyof T]: key extends keyof U ? T[key] : never;
};

type xyz = Subset<'push' | 'scale down', animation>
`);

file.forEachChild(child => {
  // if (Node.isConditionalTypeNode(child)) {

  // }
  if (Node.isTypeAliasDeclaration(child)) {
    const n = child.getNameNode();
    console.log(n.getType().getText())
    // const n = child.getTypeNode();
    // if (Node.isTypeReferenceNode(n)) {
    //   console.log(project.getTypeChecker())
    //   console.log(project.getTypeChecker().getApparentType(n.getType()).getText())
    //   // const d = n.getTypeName().getSymbol().getDeclarations()[0];
    //   // // @ts-ignore
    //   // const tp = d.getTypeParameters();
    //   // console.log(tp)
    // }
    // console.log(child.getTypeNode().getKindName())
    // console.log(child.getTypeNode().getText())
    console.log('---')
  }
  // console.log(Node.isConditionalTypeNode(child))
  // console.log(child.getKindName())
  // console.log(child.getText() + ' - ' + child.getType().getText())
});

console.log('\n -------------------------- \n');